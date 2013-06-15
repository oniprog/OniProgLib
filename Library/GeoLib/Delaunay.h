///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	ドロネー分割
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>
#include <vector>
#include <tuple>
#include <algorithm>
#include "boost/random.hpp"

#include "GeoLib/Point_2.h"
#include "GeoLib/Math.h"

namespace GeoLib {

namespace Detail {

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

}

// 2次元ドロネー分割
template< class _Point_2 = GeoLib::Point_2>
class Delaunay2D {
public:
    typedef std::vector<GeoLib::Point_2>    ZPoint2List;
    typedef std::vector<int>                ZResultList;
    typedef ZResultList::iterator           iterator;
    typedef ZResultList::const_iterator     const_iterator;

    Delaunay2D() : m_real(0,1), m_rand( m_gen, m_real ) {}

    template<class _Iterator>
    bool Apply( _Iterator begin, _Iterator end) {

        m_oData.Clear();
        m_listPoint.clear();

        // Ghost Point
        m_listPoint.push_back( Point_2( std::numeric_limits<double>::min(), std::numeric_limits<double>::min() ) );

        // 摂動を点に加える
        {        
            auto ii = begin, ii_end = end;
            for(; ii != ii_end; ++ii) {
                m_listPoint.push_back( RandPoint_2(*ii) );
            }
        }

        if (m_listPoint.size() < 4 ) 
            return false; // 点の数が足りません

        // Randomize化する
        m_listOrder.resize( m_listPoint.size() );
        auto &listOrder = m_listOrder;
        for( int it=0; it<static_cast<int>(m_listPoint.size()); ++it ) {
            listOrder[it] = it;
        }
        std::random_shuffle( listOrder.begin()+1, listOrder.end() );

        // 初期三角形を作成する
        auto pt1 = m_listPoint[listOrder[1]];
        auto pt2 = m_listPoint[listOrder[2]];
        int i3=3;
        double dArea;
        for( i3=3; i3<static_cast<int>(m_listPoint.size()); ++i3) {
         
            dArea = GeoLib::OrientExact_2( pt1, pt2, m_listPoint[listOrder[i3]] );
            if ( dArea ) {
                break;
            }
        }
        if ( i3 == (int)m_listPoint.size() )
            return false;   // 初期三角形が作れない

        // 作成
        if ( dArea > 0 ) {
            m_oData.AddTriangle(1, 2, i3);
            m_oData.AddTriangle(0, 2, 1);
            m_oData.AddTriangle(0, 1, i3);
            m_oData.AddTriangle(0, i3, 2);

            m_triBef = Detail::Triangle( 1, 2, i3 );
        }
        else {
            m_oData.AddTriangle(2, 1, i3);
            m_oData.AddTriangle(0, 1, 2);
            m_oData.AddTriangle(0, 2, i3);
            m_oData.AddTriangle(0, i3, 1);

            m_triBef = Detail::Triangle( 2, 1, i3 );
        }

        for( int i4=3; i4<static_cast<int>(m_listPoint.size()); ++i4 ) {

            if ( i4 == i3 )
                continue;   // すでに使用済みならば何もしない

            auto poiInsert = m_listPoint[ listOrder[i4] ];
            
            // 点が所属する三角形を求める
            Detail::Triangle triCur = m_triBef;
            for( ; ; ) {

                auto pt1 = m_listPoint[ listOrder[triCur.i1] ];
                auto pt2 = m_listPoint[ listOrder[triCur.i2] ];
                auto pt3 = m_listPoint[ listOrder[triCur.i3] ];

                bool bMove = false;
                if ( !bMove && triCur.i1 && triCur.i2 ) {

                    double dArea = GeoLib::OrientExact_2( pt1, pt2, poiInsert );
                    if ( dArea < 0 ) {
                        triCur.i3 = m_oData.Adjacent( triCur.i2, triCur.i1 );
                        std::swap( triCur.i1, triCur.i2 );
                        bMove = true;
                    }
                }
                if ( !bMove && triCur.i2 && triCur.i3 ) {

                    double dArea = GeoLib::OrientExact_2( pt2, pt3, poiInsert );
                    if ( dArea < 0 ) {
                        triCur.i1 = m_oData.Adjacent( triCur.i3, triCur.i2 );
                        std::swap( triCur.i2, triCur.i3 );
                        bMove = true;
                    }
                }
                if ( !bMove && triCur.i3 && triCur.i1 ) {

                    double dArea = GeoLib::OrientExact_2( pt3, pt1, poiInsert );
                    if ( dArea < 0 ) {
                        triCur.i2 = m_oData.Adjacent( triCur.i1, triCur.i3 );
                        std::swap( triCur.i3, triCur.i1 );
                        bMove = true;
                    }
                }

                if ( !bMove ) {
                    break;
                }
            }

            m_oData.DeleteTriangle( triCur.i1, triCur.i2, triCur.i3 );

            DigCavity( i4, triCur.i1, triCur.i2 );
            DigCavity( i4, triCur.i2, triCur.i3 );
            DigCavity( i4, triCur.i3, triCur.i1 );
        }

        // 結果を得る
        std::set< std::tuple<int,int,int> > setAlready;
        {
            auto ii = m_oData.cbegin(), ii_end = m_oData.cend();
            for(; ii != ii_end; ++ii ) {

                int i1 = std::get<0>( ii->first );
                int i2 = std::get<1>( ii->first );
                int i3 = ii->second;

                if ( setAlready.find( std::make_tuple(i1,i2,i3) ) != setAlready.end() )
                    continue;

                setAlready.insert( std::make_tuple(i1,i2,i3) );
                setAlready.insert( std::make_tuple(i3,i1,i2) );
                setAlready.insert( std::make_tuple(i2,i3,i1) );

                if ( i1 == 0 || i2 == 0 || i3 == 0 )
                    continue;

                m_listResult.push_back(m_listOrder[i1]-1);
                m_listResult.push_back(m_listOrder[i2]-1);
                m_listResult.push_back(m_listOrder[i3]-1);
            }
        }

        return true;
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

private:
    // 摂動を点に加える
    Point_2 RandPoint_2( const _Point_2 &pP ) {

        return Point_2( (1+m_rand()*1E-8)*get<0>(pP)+m_rand()*1e-8, (1+m_rand()*1E-8)*get<1>(pP)+m_rand()*1e-8);
    }

    // 
    void    DigCavity( int u, int v, int w, int nLevel=0) {

        int x = m_oData.Adjacent(w,v);
        if ( x < 0 )
            return;

        auto pt1 = m_listPoint[ m_listOrder[u] ];
        auto pt2 = m_listPoint[ m_listOrder[v] ];
        auto pt3 = m_listPoint[ m_listOrder[w] ];
        auto pt4 = m_listPoint[ m_listOrder[x] ];

        bool bInCircle = false;

        if ( v == 0 ) {
            bInCircle = OrientExact_2( pt3, pt4, pt1 ) < 0;
        }
        else if ( w == 0 ) {
            bInCircle = OrientExact_2( pt4, pt2, pt1 ) < 0;
        }
        else if ( x == 0 ) {
            bInCircle = OrientExact_2( pt2, pt3, pt1 ) < 0;
        }
        else {
            bInCircle = InCircleExact(pt1,pt2,pt3,pt4) > 0;
        }

        if ( bInCircle ) {

            m_oData.DeleteTriangle(w,v,x);
            DigCavity(u,v,x, nLevel+1);
            DigCavity(u,x,w, nLevel+1);
        }
        else {
            m_oData.AddTriangle(u,v,w);
            m_triBef = Detail::Triangle( u, v, w );
        }
    }
private:
    boost::mt19937              m_gen;
    boost::uniform_real<>       m_real;
    boost::variate_generator< boost::mt19937&, boost::uniform_real<>> m_rand;
    ZPoint2List                 m_listPoint;
    ZResultList                 m_listOrder;
    Detail::TriangleData        m_oData;
    Detail::Triangle            m_triBef;

    std::vector<int>            m_listResult;
};

};	// namespace GeoLib
