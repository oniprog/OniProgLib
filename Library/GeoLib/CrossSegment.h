///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	線分同士の交差と近傍点の同一化
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>
#include <algorithm>
#include "boost/random.hpp"
#include "GeoLib/Point_2.h"
#include "GeoLib/Math.h"

#include "GeoLib/Math.h"

namespace GeoLib {

// 2つの線分に交差があるかを判定する
template<class _Point_2>
bool IsCrossSegment_2( const _Point_2 &p1, const _Point_2 &p2, const _Point_2 &p3, const _Point_2 &p4 ) {

    double dSide1 = OrientExact_2( p1, p2, p3 );
    double dSide2 = OrientExact_2( p1, p2, p4 );
    if ( dSide1 * dSide2 > 0 )
        return false;

    double dSide3 = OrientExact_2( p3, p4, p1 );
    double dSide4 = OrientExact_2( p3, p4, p2 );
    if ( dSide3 * dSide4 > 0 )
        return false;

    if( (dSide1 == 0 || dSide2 == 0) && ( dSide3 == 0 && dSide4 == 0 ) )
        return false;

    return true;
}

// 2つの直線の交点を得る
template<class _Point_2>
bool GetCrossLine_2( _Point_2 &poiCross, const _Point_2 &p1, const _Point_2 &p2, const _Point_2 &p3, const _Point_2 &p4 ) {

    auto N1 = p2 - p1;
    auto N2 = p4 - p3;

    auto div = CrossProduct_2(N1,N2);
    if ( div == 0.0 )
        return false;

    double t = (-CrossProduct_2(N1, p1) + CrossProduct_2(N1, p3)) / -div;
    double s = (-CrossProduct_2(N2, p1) + CrossProduct_2(N2, p3)) / -div;

    auto pC1 = (N1 * s + p1);
    auto pC2 = (N2 * t + p3);
    poiCross = (pC1+pC2) * 0.5;
    return true;
}

// 点間の距離を計算する
template<class _Point_2>
double  GetDistancePP_2( const _Point_2 &p1, const _Point_2 &p2 ) {
    return hypot( get<0>(p1) - get<0>(p2), get<1>(p1) - get<1>(p2) );
}

namespace Detail {

// 方向を計算するためのクラス
// 縮退が起きるのを避けるため乱数を用いる
class Direction {
public:
    Direction(const Point_2 &poiBase_) : m_poiBase(poiBase_) {

        auto gen = boost::mt19937();
        auto dist = boost::uniform_real<>(1E-8,1);
        auto rand = boost::variate_generator<boost::mt19937&, boost::uniform_real<>>(gen, dist);

        m_poiDir.Define( rand(), rand() );
        m_poiDir.Normalize();
    }

    // X軸方向を調整する
    double  operator()( const Point_2 &poi ) const {
        return DotProduct_2( poi - m_poiBase, m_poiDir );
    }

private:
    GeoLib::Point_2     m_poiDir;
    GeoLib::Point_2     m_poiBase;
};
}

/*
    2点ずつ組で与える．両方とも同じ点のときは，点となる．
    異なるときは，直線と判定される．

    計算結果も同じ．ただし，ポインタとなっている点が異なる．
    ポインタはこのクラスが破棄されるとき，破壊される．

    平行な直線には対処できていない．．．
*/

template<class _Point_2>
class CrossSegment_2T {
public:
    typedef std::vector<_Point_2*>                   ZResultList;
    typedef typename ZResultList::iterator           iterator;
    typedef typename ZResultList::const_iterator     const_iterator;

    CrossSegment_2T(double dEps_ = 1E-3) : m_dEps(dEps_) {}
    ~CrossSegment_2T() {
        DeleteAll();
    }

    // すべての結果をクリアする
    void    DeleteAll() {
        std::for_each( m_listHold.begin(), m_listHold.end(), [&](Point_2 *pP) { delete pP; } );
        m_listResult.clear();
        m_listHold.clear();
    }

    // アルゴリズムを実行する
    template<class _Iterator>
    bool        Apply( _Iterator begin, _Iterator end) {

        int nSize = std::distance( begin, end );
        if ( nSize & 1 )
            return false;   // 奇数個はだめ

        std::vector<int> listAppear(nSize);    ///< 出現順
        std::vector<int> listOrder;     ///< 並び順

        // 一時領域にコピーする
        std::vector<_Point_2> listInput;
        listInput.reserve(nSize);
        std::copy( begin, end, std::back_inserter(listInput) );

        // 
        Detail::Direction dir(*begin);

        for( int it=0; it<nSize; ++it ) { listAppear[it] = it; }

        // 指定軸でソートする
        std::sort( listAppear.begin(), listAppear.end(), [&](int i1, int i2)->bool {
            return dir(listInput[i1]) < dir(listInput[i2]); 
        });

        // 線分を分割する点を記録する
        std::map<int, ZResultList > mapDivide;

        //
        std::set<int> setAppear;
        auto ii = listAppear.begin(), ii_end = listAppear.end(); 
        for(; ii != ii_end; ++ii ) {

            auto nAppear = *ii;
            if ( setAppear.find(nAppear/2) == setAppear.end() ) {
        
                // 出現したのでどこかに挿入する
                setAppear.insert( nAppear/2);
            }
            else {

                // 消す
                setAppear.erase( nAppear/2 );

                // 
                int nV3 = (nAppear/2)*2;
                int nV4 = nV3+1;
                auto v3 = listInput[nV3];
                auto v4 = listInput[nV4];

                if ( IsSamePoint_2(v3, v4)) {

                    // 点を追加する
                    Point_2 *pP = AddPoint(v3);
                }
                else {

                    // 消えるので交差を計算する
                    // 出現しているすべての頂点に対して計算する
                    auto jj = setAppear.cbegin(), jj_end = setAppear.cend();
                    for( ; jj != jj_end; ++jj ) {

                        auto nV1 = (*jj)*2;
                        auto nV2 = nV1+1;

                        auto v1 = listInput[nV1];
                        auto v2 = listInput[nV2];

                        if ( IsCrossSegment_2(v1,v2,v3,v4)) {
                            Point_2 poiCross;
                            GetCrossLine_2( poiCross, v1,v2,v3,v4);

                            // 分割点を記録する
                            Point_2 *pCross = AddPoint(poiCross);
                            mapDivide[nV1/2].push_back( pCross );
                            mapDivide[nV3/2].push_back( pCross );
                        }
                    }
                }
            }
        }

        // 線分を分割していく
        {
            for( int il=0; il<(int)nSize; il+=2) {
             
                auto v1 = listInput[il+0];
                auto v2 = listInput[il+1];
                if ( IsSamePoint_2(v1, v2 ) )
                    continue;

                auto pV1 = AddPoint(v1);
                auto pV2 = AddPoint(v2);

                auto listDiv = mapDivide[il/2];
                listDiv.push_back(pV1);
                listDiv.push_back(pV2);

                std::sort( listDiv.begin(), listDiv.end(), [&]( Point_2 *pP1, Point_2 *pP2)->bool {
                    return dir(*pP1) < dir(*pP2);
                });

                for( int it=0; it<static_cast<int>(listDiv.size()) ; ++it ) {
                
                    if ( it > 0 && it < static_cast<int>(listDiv.size()-1)) {
                        m_listResult.push_back( listDiv[it] );
                    }
                    m_listResult.push_back( listDiv[it] );
                }
            }
        }
    }



    iterator        begin() {
        return m_listResult.begin();
    }
    iterator        end() {
        return m_listResult.end();
    }
    const_iterator  cbegin() const {
        return m_listResult.cbegin();
    }
    const_iterator  cend() const {
        return m_listResult.cend();
    }
    int         size() const {
        return static_cast<int>( m_listResult.size() );
    }
private:

    bool        IsSamePoint_2( const _Point_2 &p1, const _Point_2 &p2 ) {
        return GetDistancePP_2(p1, p2 ) < m_dEps;
    }

    Point_2     *AddPoint( const _Point_2 &poi ) {
        bool bDmy;
        return AddPoint( bDmy, poi );
    }

    Point_2     *AddPoint( bool &bNew, const _Point_2 &poi ) {

        bNew = false;
        auto ii = m_listHold.begin(), ii_end = m_listHold.end();
        for( ; ii != ii_end; ++ii ) {
            if ( IsSamePoint_2(**ii, poi) )
                return *ii; // すでに登録済み
        }
        auto pNew = new Point_2(poi);
        // 点なので2つ追加
        m_listResult.push_back(pNew);
        m_listResult.push_back(pNew);
        m_listHold.push_back(pNew);
        bNew = true;
        return pNew;
    }

private:
    double              m_dEps;         ///< 丸め値
    ZResultList         m_listResult;   ///< 計算結果
    ZResultList         m_listHold;     ///< 点保持用
};

typedef CrossSegment_2T<GeoLib::Point_2>    CrossSegment_2;

};	// namespace GeoLib
