///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	距離計算用
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

/*
    略記号の意味
    P       点
    L       直線
    S       線分
    A       平面
*/

// 点間の距離を計算する
template<class _Point_2>
double  GetDistancePP_2( const _Point_2 &p1, const _Point_2 &p2 ) {
    return hypot( get<0>(p1) - get<0>(p2), get<1>(p1) - get<1>(p2) );
}

template<class _Point_2>
double	GetDistancePL_2( const _Point_2 &pl1, const _Point_2 &pl2, const _Point_2 &poi ) {
	double dT = DotProduct_2( poi - pl1, pl2 - pl1 ) / DotProduct_2( pl2 - pl1, pl2 - pl1 );
	return GetDistancePP_2( pl1 * (1-dT) + pl2 * dT, poi );
}

};	// namespace GeoLib
