///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	交点計算
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GeoLib/Point_2.h"
#include "GeoLib/Math.h"

namespace GeoLib {

///////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////
// 2つの直線の交点を得る
template<class _Point_2>
bool GetCrossLine_2( _Point_2 &poiCross, const _Point_2 &p1, const _Point_2 &p2, const _Point_2 &p3, const _Point_2 &p4 ) {

    auto N1 = p2 - p1;
    auto N2 = p4 - p3;

    auto div = CrossProduct_2(N1,N2);
    if ( div == 0.0 )
        return false;

    auto t = (-CrossProduct_2(N1, p1) + CrossProduct_2(N1, p3)) / -div;
    auto s = (-CrossProduct_2(N2, p1) + CrossProduct_2(N2, p3)) / -div;

    auto pC1 = (N1 * s + p1);
    auto pC2 = (N2 * t + p3);
    poiCross = (pC1+pC2) * 0.5;
    return true;
}

};	// namespace GeoLib
