///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	数学関係
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <limits>
#include "boost/multiprecision/gmp.hpp"

namespace GeoLib {

///////////////////////////////////////////////////////////////////////////////////
// 2次の行列式
template<class _Value>
_Value	Determinant2x2( const _Value &v1, const _Value &v2, const _Value &v3, const _Value &v4 ) {
	return v1 * v4 - v2 * v3;
}

///////////////////////////////////////////////////////////////////////////////////
// 3次の行列式
template<class _Value>
_Value	Determinant3x3( 
	const _Value &v1, const _Value &v2, const _Value &v3, 
	const _Value &v4, const _Value &v5, const _Value &v6, 
	const _Value &v7, const _Value &v8, const _Value &v9 
) {
	return v1 * Determinant2x2( v5, v6, v8, v9) - v2 * Determinant2x2( v4, v6, v7, v9) + v3 * Determinant2x2( v4, v5, v7, v8 );
}

///////////////////////////////////////////////////////////////////////////////////
// 3点のp1, p2, p3の曲がり方を判定する
template<class _Point_2 >
typename _Point_2::value_type Orient_2( const _Point_2 &p1, const _Point_2 &p2, const _Point_2 &p3 ) {

	return Determinant2x2( 
		get<0>(p1) - get<0>(p3), p2.GetX() - p3.GetX(), 
		p1.GetY() - p3.GetY(), p2.GetY() - p3.GetY() 
	);
}

///////////////////////////////////////////////////////////////////////////////////
// 4点のp1, p2, p3, p4の位置関係を判定する
template<class _Point_3>
typename _Point_3::value_type	Orient_3( const _Point_3 &p1, const _Point_3 &p2, const _Point_3 &p3, const _Point_3 &p4 ) {

	return Determinant3x3( 
		p1.GetX() - p4.GetX(), p2.GetX() - p4.GetX(), p3.GetX() - p4.GetX(), 
		p1.GetY() - p4.GetY(), p2.GetY() - p4.GetY(), p3.GetY() - p4.GetY(),
		p1.GetZ() - p4.GetZ(), p2.GetZ() - p4.GetZ(), p3.GetZ() - p4.GetZ()
	);
}

///////////////////////////////////////////////////////////////////////////////////
// 2次の行列式の絶対値
template<class _Value>
_Value	DeterminantAbs2x2( const _Value &v1, const _Value &v2, const _Value &v3, const _Value &v4 ) {
	return std::fabs(v1 * v4) + std::fabs(v2 * v3);
}

///////////////////////////////////////////////////////////////////////////////////
// 3次の行列式の絶対値
template<class _Value>
_Value	DeterminantAbs3x3( 
	const _Value &v1, const _Value &v2, const _Value &v3, 
	const _Value &v4, const _Value &v5, const _Value &v6, 
	const _Value &v7, const _Value &v8, const _Value &v9 
) {
	return std::fabs(v1) * DeterminantAbs2x2( v5, v6, v8, v9) + std::fabs(v2) * DeterminantAbs2x2( v4, v6, v7, v9) + std::fabs(v3) * DeterminantAbs2x2( v4, v5, v7, v8 );
}

///////////////////////////////////////////////////////////////////////////////////
// 絶対値
template<class _Point_2 >
typename _Point_2::value_type	OrientAbs_2( const _Point_2 &p1, const _Point_2 &p2, const _Point_2 &p3 ) {

	return DeterminantAbs2x2( 
		p1.GetX() - p3.GetX(), p2.GetX() - p3.GetX(), 
		p1.GetY() - p3.GetY(), p2.GetY() - p3.GetY() 
		);
}

///////////////////////////////////////////////////////////////////////////////////
// 3点のp1, p2, p3の曲がり方を判定する(適応型無誤差演算版)
template<class _Point_2>
typename _Point_2::value_type	OrientExact_2( const _Point_2 &p1, const _Point_2 &p2, const _Point_2 &p3 ) {

	// 誤差を見積もりつつ計算する
	typedef _Point_2::value_type V;
	V dOrient = Orient_2( p1, p2, p3 );
	V dGosaValue = OrientAbs_2( p1, p2, p3 ) * std::numeric_limits<double>::epsilon() * 5;
	if ( std::fabs(dOrient) > dGosaValue )
		return dOrient;	// 計算値が誤差よりも大きいのでOK

	// 無誤差演算で計算する
	typedef Point_2T<GeoLib::Rational> RP;
	return GeoLib::static_rational_cast<double>( Orient_2( RP(p1), RP(p2), RP(p3) ) );

}

///////////////////////////////////////////////////////////////////////////////////
/// InCircleテスト
template<class _Point_2>
typename _Point_2::value_type	InCircle( const _Point_2 &p1, const _Point_2 &p2, const _Point_2 &p3, const _Point_2 &p4 ) {

	double adx = get<0>(p1) - get<0>(p4);
	double ady = get<1>(p1) - get<1>(p4);
	double bdx = get<0>(p2) - get<0>(p4);
	double bdy = get<1>(p2) - get<1>(p4);
	double cdx = get<0>(p3) - get<0>(p4);
	double cdy = get<1>(p3) - get<1>(p4);

	return GeoLib::Determinant3x3(
		get<0>(p1) - get<0>(p4), get<1>(p1) - get<1>(p4), adx*adx + ady*ady,
		get<0>(p2) - get<0>(p4), get<1>(p2) - get<1>(p4), bdx*bdx + bdy*bdy,
		get<0>(p3) - get<0>(p4), get<1>(p3) - get<1>(p4), cdx*cdx + cdy*cdy
    );
}

///////////////////////////////////////////////////////////////////////////////////
/// InCircleテスト
template<class _Point_2>
typename _Point_2::value_type	InCircleExact( const _Point_2 &p1, const _Point_2 &p2, const _Point_2 &p3, const _Point_2 &p4 ) {

    double adx = get<0>(p1) - get<0>(p4);
    double ady = get<1>(p1) - get<1>(p4);
    double bdx = get<0>(p2) - get<0>(p4);
    double bdy = get<1>(p2) - get<1>(p4);
    double cdx = get<0>(p3) - get<0>(p4);
    double cdy = get<1>(p3) - get<1>(p4);

    double dDet = GeoLib::Determinant3x3(
        get<0>(p1) - get<0>(p4), get<1>(p1) - get<1>(p4), adx*adx + ady*ady,
        get<0>(p2) - get<0>(p4), get<1>(p2) - get<1>(p4), bdx*bdx + bdy*bdy,
        get<0>(p3) - get<0>(p4), get<1>(p3) - get<1>(p4), cdx*cdx + cdy*cdy
    );

    double dDetAbs = GeoLib::DeterminantAbs3x3(
        get<0>(p1) - get<0>(p4), get<1>(p1) - get<1>(p4), adx*adx + ady*ady,
        get<0>(p2) - get<0>(p4), get<1>(p2) - get<1>(p4), bdx*bdx + bdy*bdy,
        get<0>(p3) - get<0>(p4), get<1>(p3) - get<1>(p4), cdx*cdx + cdy*cdy
    );

	if ( std::fabs(dDet) > dDetAbs*std::numeric_limits<double>::epsilon()*12 )
		return dDet;

    {
		typedef GeoLib::Rational RA;
        RA adx(RA(get<0>(p1)) - RA(get<0>(p4)));
		RA ady(RA(get<1>(p1)) - RA(get<1>(p4)));
		RA bdx(RA(get<0>(p2)) - RA(get<0>(p4)));
		RA bdy(RA(get<1>(p2)) - RA(get<1>(p4)));
		RA cdx(RA(get<0>(p3)) - RA(get<0>(p4)));
		RA cdy(RA(get<1>(p3)) - RA(get<1>(p4)));
	
		RA dDet = GeoLib::Determinant3x3(
            RA(get<0>(p1)) - RA(get<0>(p4)), RA(get<1>(p1)) - RA(get<1>(p4)), adx*adx + ady*ady,
            RA(get<0>(p2)) - RA(get<0>(p4)), RA(get<1>(p2)) - RA(get<1>(p4)), bdx*bdx + bdy*bdy,
            RA(get<0>(p3)) - RA(get<0>(p4)), RA(get<1>(p3)) - RA(get<1>(p4)), cdx*cdx + cdy*cdy
		);
		return static_rational_cast<double>(dDet);
    }
}

};	// namespace GeoLib
