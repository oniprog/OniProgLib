///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	有理数クラスのラッパー	
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <limits>
#include "boost/multiprecision/gmp.hpp"

namespace GeoLib {

// expression templateをOFFにする
typedef boost::multiprecision::number< boost::multiprecision::gmp_rational, boost::multiprecision::et_off> Rational;

template<class T>
T static_rational_cast( const Rational &r ) {
	return r;
}
template<class T>
T static_rational_cast( Rational &&r ) {
	return r;
}

// doubleへのCASTを定義する
template<>
inline
double static_rational_cast<double>(const Rational&r ) {
	return mpq_get_d( r.backend().data() );
}

// doubleへのCASTを定義する
template<>
inline
double static_rational_cast<double>( Rational&& r ) {
	return mpq_get_d( r.backend().data() );
}

};	// namespace GeoLib
