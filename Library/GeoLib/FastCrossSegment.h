///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	線分同士の交差と近傍点の同一化
			分割統治法を用いている．
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>
#include <algorithm>
#include "boost/random.hpp"
#include "GeoLib/Point_2.h"
#include "GeoLib/Math.h"

#include "GeoLib/CrossPoint.h"
#include "GeoLib/Distance.h"

namespace GeoLib {

namespace Detail {

// 指定された線分上への投影位置を返すクラス
template<class _Point_2>
class ProjectionOnLine {
public:
	ProjectionOnLine( const _Point_2 &pl1, const _Point_2 &pl2 ) : m_pl1(pl1), m_pl2(pl2) {}
	double		operator()( const _Point_2 &poi ) {
		double dT = DotProduct_2( poi - m_pl1, m_pl2 - m_pl1 ) / DotProduct_2( m_pl2 - m_pl1, m_pl2 - m_pl1 );
		return dT;
	}

private:
	_Point_2	m_pl1, m_pl2;
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
class FastCrossSegment_2T {
public:
    typedef std::vector<_Point_2*>                  ZResultList;
    typedef typename ZResultList::iterator          iterator;
    typedef typename ZResultList::const_iterator    const_iterator;
	typedef std::vector<_Point_2>					ZPointList;

    FastCrossSegment_2T(double dEpsPoint_ = 1E-3, double dEpsLine = 1E-6, int nBruteForceLimit=10, int nMaxLevel = 40) 
		: m_real(0,1), m_rand( m_gen, m_real ),
		  m_dEpsPoint(dEpsPoint_), m_dEpsLine(dEpsLine), m_nBruteForceLimit(nBruteForceLimit), m_nMaxLevel(nMaxLevel) {}
    ~FastCrossSegment_2T() {
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

		// 最小値と最大値を求める
		_Point_2 poiMin, poiMax;
		double dBigValue = std::numeric_limits<double>::max();
		poiMin.Define( dBigValue, dBigValue );
		poiMax.Define( -dBigValue, -dBigValue );
		auto ii = begin, ii_end = end;
		for(; ii != ii_end; ++ii ) {
			poiMin = GetMinPoint_2( poiMin, *ii );
			poiMax = GetMaxPoint_2( poiMax, *ii );
		}

		ZPointList A( begin, end );
		A = DivideSegment( A, A, poiMin, poiMax, 0.5+m_rand()*1E-6, 0 );	
		if ( m_nMaxLevel > 0 ) {
			// 境界部分で判定をミスするのを避けるため　二重に処理をする
			A = DivideSegment( A, A, poiMin, poiMax, 0.3+m_rand()*1E-6, 0 );	
		}
		
		// ポインタに変換する
		std::map<_Point_2, _Point_2*> mapPoint;
		ZResultList listResult;
		std::for_each( A.cbegin(), A.cend(), [&](const _Point_2 &pt ) {
			auto ii_find = mapPoint.find( pt );
			if ( ii_find != mapPoint.end() ) {
				listResult.push_back( ii_find->second );
			}
			else {
				auto pNewPoi = new _Point_2( pt );
				mapPoint[pt] = pNewPoi;
				listResult.push_back( pNewPoi );
			}
		});

		// 同一直線を省きつつコピーする
		typedef std::tuple<_Point_2*, _Point_2*> ZPointPair;
		std::set<ZPointPair> setAlready;
		for( int it=0; it<static_cast<int>(listResult.size()); it+=2 ) {
			auto ps1 = listResult[it+0];
			auto ps2 = listResult[it+1];
			if ( setAlready.find( std::make_tuple(ps1,ps2) ) != setAlready.end() )
				continue;

			m_listResult.push_back( ps1 );
			m_listResult.push_back( ps2 );

			setAlready.insert( std::make_tuple(ps1,ps2) );
			setAlready.insert( std::make_tuple(ps2,ps1) );
		}
		return true;
	}

	// 総当たり計算
	ZPointList	BruteForce( const ZPointList &A, const ZPointList &B ) {

		ZPointList C = A;
		ZPointList listRet;
		while( C.size() > 0) {
			ZPointList NextC;
			for( int i1=0; i1<static_cast<int>(C.size()); i1+=2) {
				auto ps1 = C[i1+0];
				auto ps2 = C[i1+1];
				bool bToRet = true;
				bool bIsPoint = ps1 == ps2;
				for( int i2=0; i2<static_cast<int>(B.size()); i2+=2 ) {
					auto ps3 = B[i2+0];
					auto ps4 = B[i2+1];

					bool bIsPoint2 = ps3 == ps4;

					if ( bIsPoint && bIsPoint2 ) {
						if ( IsSamePoint_2( ps1, ps3 ) ) {
							ps1 = ps3;
							ps2 = ps3;
							break;	// 同じ点を発見したので終了
						}
						continue;
					}
					else if ( bIsPoint && !bIsPoint2 ) {
						continue;	// 何もしない
					}

					bool bParallel = false;
					double dDistLine1 = GetDistancePL_2( ps1, ps2, ps3 );
					double dDistLine2 = GetDistancePL_2( ps1, ps2, ps4 );
					if ( dDistLine1 < m_dEpsLine && dDistLine2 < m_dEpsLine ) {

						double dDistLine3 = bIsPoint2 ? 0 : GetDistancePL_2( ps3, ps4, ps1 );
						double dDistLine4 = bIsPoint2 ? 0 : GetDistancePL_2( ps3, ps4, ps2 );
						if ( dDistLine3 < m_dEpsLine && dDistLine4 < m_dEpsLine ) {

							// 平行な直線発見
							bParallel = true;
							Detail::ProjectionOnLine<_Point_2> projection( ps1, ps2 );
							double dT1 = projection(ps3);
							double dT2 = projection(ps4);
							if ( dT1 > dT2 ) std::swap( dT1, dT2 );
							std::vector<_Point_2> listAddPoint;
							if ( dT1 >= 0 && dT1 <= 1 ) {
								auto poiNew1 = ps1 * (1-dT1) + ps2 * dT1;
								if ( !IsSamePoint_2(ps1, poiNew1) && !IsSamePoint_2( ps2, poiNew1) ) {
									listAddPoint.push_back( poiNew1 );
								}
							}
							if ( dT2 >= 0 && dT2 <= 1 ) {
								auto poiNew2 = ps1 * (1-dT2) + ps2 * dT2;
								if ( !IsSamePoint_2(ps1, poiNew2) && !IsSamePoint_2( ps2, poiNew2) ) {
									listAddPoint.push_back( poiNew2 );
								}
							}
							if ( !listAddPoint.empty() ) {
								NextC.push_back( ps1 );
								std::for_each( listAddPoint.cbegin(), listAddPoint.cend(), [&](const _Point_2 &pt) { NextC.push_back(pt); NextC.push_back(pt); } );
								NextC.push_back( ps2 );
								bToRet = false;
								break;
							}
						}
					}

					// 平行な直線でなかった場合
					if ( !bIsPoint2 && !bParallel && IsCrossSegment_2( ps1, ps2, ps3, ps4 ) ) {
						// 交差するとき
						_Point_2 poiCross;
						GetCrossLine_2( poiCross, ps1, ps2, ps3, ps4 );
						if ( !IsSamePoint_2( ps1, poiCross ) && !IsSamePoint_2(ps2, poiCross ) ) {
							NextC.push_back(ps1);	NextC.push_back(poiCross);
							NextC.push_back(poiCross);	NextC.push_back( ps2 );
							bToRet = false;
							break;
						}
					}
				}
				if ( bToRet ) {
					// 交差などなかったので，そのまま出力
					listRet.push_back( ps1 );
					listRet.push_back( ps2 );
				}
			}
			C.swap( NextC );
		}
		return listRet;
	}

	// X軸で線分群を分ける
	void ClassifySegmentByX( ZPointList &C, ZPointList &D, ZPointList &E, const ZPointList &A, double dSplitX ) {
		for( int it=0; it<static_cast<int>(A.size()); it+=2 ) {
			auto ps1 = A[it+0];
			auto ps2 = A[it+1];
			if ( get<0>(ps1) < dSplitX && get<0>(ps2) < dSplitX ) {
				C.push_back(ps1);	C.push_back(ps2);
			} else if ( get<0>(ps1) > dSplitX && get<0>(ps2) > dSplitX ) {
				D.push_back(ps1);	D.push_back(ps2);
			} else {
				E.push_back(ps1);	E.push_back(ps2);
			}
		}
	}

	// Y軸で線分群を分ける
	void ClassifySegmentByY( ZPointList &C, ZPointList &D, ZPointList &E, const ZPointList &A, double dSplitY ) {
		for( int it=0; it<static_cast<int>(A.size()); it+=2 ) {
			auto ps1 = A[it+0];
			auto ps2 = A[it+1];
			if ( get<1>(ps1) < dSplitY && get<1>(ps2) < dSplitY ) {
				C.push_back(ps1);	C.push_back(ps2);
			} else if ( get<1>(ps1) > dSplitY && get<1>(ps2) > dSplitY ) {
				D.push_back(ps1);	D.push_back(ps2);
			} else {
				E.push_back(ps1);	E.push_back(ps2);
			}
		}
	}

	// 線分を組み合わせる
	ZPointList CombineSegment( const ZPointList &A, const ZPointList &B ) {
		auto ret = A;
		ret.reserve( A.size()+B.size() );
		std::copy( B.cbegin(), B.cend(), std::back_inserter(ret));
		return ret;
	}

	ZPointList CombineSegment( const ZPointList &A, const ZPointList &B, const ZPointList &C ) {
		auto ret = A;
		ret.reserve( A.size()+B.size()+C.size() );
		std::copy( B.cbegin(), B.cend(), std::back_inserter(ret));
		std::copy( C.cbegin(), C.cend(), std::back_inserter(ret));
		return ret;
	}

	// 分割統治法を用いた計算
	ZPointList	DivideSegment( const ZPointList &A, const ZPointList &B, const _Point_2 &poiMin, const _Point_2 &poiMax, double dSplitRatio, int nLevel ) {

		if ( nLevel >= m_nMaxLevel || static_cast<int>(A.size()) <= m_nBruteForceLimit ) {
			return BruteForce( A, B );
		}

		ZPointList C1, D1, E1;
		ZPointList C2, D2, E2;
		_Point_2 poiMid1, poiMid2;
		if ( nLevel & 2 ) {
			// X軸で分ける
			double dSplitX = get<0>(poiMin) * (1-dSplitRatio) + get<0>(poiMax) * dSplitRatio;
			poiMid1.Define( dSplitX, get<1>(poiMax));
			poiMid2.Define( dSplitX, get<1>(poiMin));
			ClassifySegmentByX( C1, D1, E1, A, dSplitX );
			ClassifySegmentByX( C2, D2, E2, B, dSplitX );
		}
		else {
			// Y軸で分ける
			double dSplitY = get<1>(poiMin) * (1-dSplitRatio) + get<1>(poiMax) * dSplitRatio;
			poiMid1.Define( get<0>(poiMax), dSplitY );
			poiMid2.Define( get<0>(poiMin), dSplitY );
			ClassifySegmentByY( C1, D1, E1, A, dSplitY );
			ClassifySegmentByY( C2, D2, E2, B, dSplitY );
		}

		// 再帰的に適用する
		if ( C1.empty()&& D1.empty() ) {

			return BruteForce( E1, B );
		}
		else {
			auto C3 = DivideSegment( C1, CombineSegment(C2, E2), poiMin, poiMid1, dSplitRatio, nLevel+1 );
			auto D3 = DivideSegment( D1, CombineSegment(D2, E2), poiMid2, poiMax, dSplitRatio, nLevel+1 );
			auto E3 = DivideSegment( E1, B, poiMin, poiMax, dSplitRatio, nLevel+1 );
			return CombineSegment( C3, D3, E3);
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
        return GetDistancePP_2(p1, p2 ) < m_dEpsPoint;
    }

private:
    boost::mt19937              m_gen;
    boost::uniform_real<>       m_real;
    boost::variate_generator< boost::mt19937&, boost::uniform_real<>> m_rand;
    double              m_dEpsPoint;    ///< 同一点と判定する距離
    double              m_dEpsLine;		///< 同一直線と判定する距離
	int					m_nMaxLevel;	///< 分割統治法を適用する上限レベル
	int					m_nBruteForceLimit;	///< 総当たりを行う個数
    ZResultList         m_listResult;   ///< 計算結果
    ZResultList         m_listHold;     ///< 点保持用
};

typedef FastCrossSegment_2T<GeoLib::Point_2>    FastCrossSegment_2;

};	// namespace GeoLib
