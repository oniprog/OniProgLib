///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	2次元の領域メッシュ分割(ドロネー分割ではない）
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GeoLib/Point_2.h"
#include "GeoLib/Math.h"
#include "GeoLib/Distance.h"

/*
　ドロネー分割ではない二次元領域のメッシュ分割手法
  ドロネー分割よりも高速であることが期待される．
  品質では当然，ドロネー分割よりは劣る．
*/

namespace GeoLib {

class Triangulation2D {
public:

    typedef std::vector<int>                ZResultList;
    typedef ZResultList::iterator           iterator;
    typedef ZResultList::const_iterator     const_iterator;

    iterator        begin() {
        return m_listResult.begin();
    }
    iterator        end() {
        return m_listResult.end();
    }
    const_iterator  cbegin() const {
        return m_listResult.begin();
    }
    const_iterator  cend() const {
        return m_listResult.end();
    }

    template<class _Iterator>
    void    Apply(_Iterator __begin, _Iterator __end ) {

        // 結果をクリアする
        m_dicTriangle.Clear();
        m_listResult.clear();

        // 配列にコピーする
        std::vector<Point_2> listSrc;
        std::copy( __begin, __end, std::back_inserter(listSrc));
        if ( listSrc.size() < 3 )
            return; // 無理っす

        // 元の番号を記録するため
        std::vector<int> listOrder(listSrc.size());
        for(int it=0; it<(int)listOrder.size(); ++it ) 
            listOrder[it] = it;

        // X座標順にソートする
        std::sort( listOrder.begin(), listOrder.end(), [&](int i1, int i2) {
            return get<0>( listSrc[i1] ) < get<0>( listSrc[i2] );
        });

        std::vector<int> listHead;  // 先頭を管理
        // listHeadにはY座標順にソートされて点が記録される

        std::vector<int> listL, listR;      // 縁点の記録
        // LがY軸が小さい方でRがY軸の大きい方(右から見たイメージ)

        // 一つ追加
        listHead.push_back(0);

        for( int ip=1; ip<(int)listOrder.size(); ++ip ) {

            auto poi = GetPoint(ip, listOrder, listSrc );

            // 挿入位置を探す

            auto poiH1 = GetPoint(listHead.front(), listOrder, listSrc );
            auto poiH2 = GetPoint(listHead.back(), listOrder, listSrc );

            if ( get<1>(poiH1) > get<1>(poi) ) {

                // 一番下よりも下なので，下に入れる
                listHead.insert( listHead.begin(), ip );

                // 左端が更新される
                listL.pop_back();
                listL.push_back( ip );
            }
            else if ( get<1>(poiH2) < get<1>(poi) ){

                // 一番上よりも上なので，上に入れる
                listHead.push_back( ip );

                // 右端が更新される
                listR.pop_back();
                listR.push_back( ip );
            }
            else {
                // listHeadのY座標の間で，点が入る位置を探す
                int ipp1=0, ipp2=(int)listHead.size()-1; 
                int ipp = 0;
                while(true) {

                    ipp = (ipp1+ipp2)/2;
                    auto p1 = GetPoint( listHead[ipp+0], listOrder, listSrc );
                    auto p2 = GetPoint( listHead[ipp+1], listOrder, listSrc );
                    if ( get<1>(p1) > get<1>(poi) ) {
                        ipp2 = ipp;
                    }
                    else if ( get<1>(poi) > get<1>(p2 ) ) {
                        ipp1 = ipp;
                    }
                    else {
                        break;
                    }
                }
                // 必ず見つかるはず

                // 2つの三角形の作り方を比較して，得点のよい方を採用する．
                int ib = m_dicTriangle.Adjacent( listHead[ipp+0], listHead[ipp+1] ); // 後ろ側の点

                auto pt1 = GetPoint( listHead[ipp+0], listOrder, listSrc );
                auto pt2 = GetPoint( listHead[ipp+1], listOrder, listSrc );
                auto pn1 = GetPoint( ip, listOrder, listSrc );
                auto pb1 = GetPoint( ib, listOrder, listSrc ); // listHead[ipp+0]に対応する後ろの点
                // 形状1
                double dScore1 = CalcTriangleScore( pn1, pt2, pt1 ) + (ib >= 0 ? CalcTriangleScore( pb1, pt1, pt2 ) : 0 ); 
                
                // 形状2
                double dScore2 = 
                    (ib >= 0 && Orient_2( pb1, pn1, pt2 ) > 0 && Orient_2( pb1, pt1, pn1 ) > 0 ) // 形状2が成り立つ条件をチェック 
                    ? CalcTriangleScore( pb1, pn1, pt1 ) + CalcTriangleScore( pb1, pt2, pn1 ) 
                    :  -1;

                if ( dScore1 > dScore2 ) {

                    // 形状1
                    MakeTriangle( ip, listHead[ipp+1], listHead[ipp+0] );
                    MakeTriangle( ib, listHead[ipp+0], listHead[ipp+1] );

                    // 先頭集団に点を追加
                    listHead.insert( listHead.begin() + ipp + 1, ip );
                }
                else {

                    // 形状2がよい
                    // 以前の三角形を消す(あれば)
                    DeleteTriangle( ip, listHead[ipp+1], listHead[ipp+0] );
                    DeleteTriangle( ib, listHead[ipp+0], listHead[ipp+1] );

                    //
                    MakeTriangle( ib, ip, listHead[ipp+1] );
                    MakeTriangle( ib, listHead[ipp+0], ip);

                    listHead.insert( listHead.begin() + ipp + 1, ip );
                }
            }
        }

        // 結果を作成する
        {
            //            for( auto &tri : m_dicTriangle ) {   VC2010対応のために．．．
            auto ii = m_dicTriangle.begin(), ii_end = m_dicTriangle.end();
            for( ; ii != ii_end; ++ii ) {
                auto tri = *ii;
                auto i1 = std::get<0>( tri.first );
                auto i2 = std::get<1>( tri.first );
                auto i3 = tri.second;

                if ( i1 < i2 && i1 < i3 )
                    ;
                else
                    continue;

                m_listResult.push_back( listOrder[i1] );
                m_listResult.push_back( listOrder[i2] );
                m_listResult.push_back( listOrder[i3] );
            }
        }
    }

public:
    // 三角形の品質を計算して返す
    // 1が最も品質がよい．0が最悪．
    static      double  CalcTriangleScore( const Point_2 &pt1, const Point_2 &pt2, const Point_2 &pt3 ) {

        double a = GeoLib::GetDistancePP_2(pt1, pt2);
        double b = GeoLib::GetDistancePP_2(pt2, pt3);
        double c = GeoLib::GetDistancePP_2(pt3, pt1);

        if ( a == 0 || b == 0 || c == 0 )
            return 0.0;
        else
            return (b+c-a) * (c+a-b) * (a+b-c) / (a*b*c);  
    }

private:
    class TriangleData {
    public:
        typedef std::tuple<int,int>     ZEdge;
        typedef std::map<ZEdge,int>     ZTriMap;
        typedef ZTriMap::iterator       iterator;
        typedef ZTriMap::const_iterator const_iterator;

        // u, v, w must be positive oriented triangle
        void    AddTriangle( int u, int v, int w ) {
            m_mapTri[ZEdge(u,v)] = w;
            m_mapTri[ZEdge(v,w)] = u;
            m_mapTri[ZEdge(w,u)] = v;
        }

        // u, v, w must be positive oriented triangle
        void    DeleteTriangle(int u, int v, int w) {
            m_mapTri.erase( ZEdge(u,v) );
            m_mapTri.erase( ZEdge(v,w) );
            m_mapTri.erase( ZEdge(w,u) );
        }

        // u, v, w must be positive oriented triangle
        int     Adjacent(int u, int v){
            auto ii_find = m_mapTri.find(ZEdge(u,v));
            if ( ii_find != m_mapTri.end() )
                return ii_find->second;
            else
                return -1;
        }

        void    Clear() {
            m_mapTri.clear();
        }

        iterator    begin() {
            return m_mapTri.begin();
        }
        iterator    end() {
            return m_mapTri.end();
        }
        const_iterator  cbegin() const {
            return m_mapTri.cbegin();
        }
        const_iterator  cend() const {
            return m_mapTri.cend();
        }

    private:
        ZTriMap     m_mapTri;
    };

    struct Triangle {
        int i1, i2, i3;
        Triangle(){}
        Triangle(int i1_, int i2_, int i3_ ) : i1(i1_), i2(i2_), i3(i3_) {}
    };

private:

    TriangleData    m_dicTriangle;
    ZResultList     m_listResult;

    // 三角形を作成する
    void    MakeTriangle( int i1, int i2, int i3 ) {
        if ( i1 < 0 || i2 < 0 || i3 < 0)
            return;
        m_dicTriangle.AddTriangle( i1, i2, i3 );
    }

    // 三角形を削除する
    void    DeleteTriangle( int i1, int i2, int i3 ) {
        if ( i1 < 0 || i2 < 0 || i3 < 0)
            return;
        m_dicTriangle.DeleteTriangle(i1, i2, i3 );
    }

    Point_2     GetPoint( int nIndex, std::vector<int> &listOrder, std::vector<Point_2> &listPoint ) {
        if ( nIndex < 0 )   return Point_2(0,0);
        return listPoint[listOrder[nIndex]];
    }

};

};	// namespace GeoLib
