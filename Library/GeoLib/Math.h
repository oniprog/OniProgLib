///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	数学関係
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace GeoLib {

// 2次の行列式
template<class _Value>
_Value	Determinant2x2( const _Value &v1, const _Value &v2, const _Value &v3, const _Value &v4 ) {
	return v1 * v4 - v2 * v3;
}

// 3次の行列式
template<class _Value>
_Value	Determinant3x3( 
	const _Value &v1, const _Value &v2, const _Value &v3, 
	const _Value &v4, const _Value &v5, const _Value &v6, 
	const _Value &v7, const _Value &v8, const _Value &v9 
) {
	return v1 * Determinant2x2( v5, v6, v8, v9) - v2 * Determinant2x2( v4, v6, v7, v9) + v3 * Determinant2x2( v4, v5, v7, v8 );
}


};	// namespace GeoLib
