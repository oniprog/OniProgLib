﻿///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	2次元点クラス
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace GeoLib {

template<class _Value>
class Point_2T {

public:
	Point_2T() {}	// 高速化のために未初期化
	Point_2T( const _Value &v1_, const _Value &v2_ ) {
		Define( v1_, v2_ );
	}

	void	Define( const _Value &v1_, const _Value &v2_ ) {
		m_v[0] = v1_;	m_v[1] = v2_;
	}

	_Value	Get(int nIndex) const {
		return m_v[nIndex];
	}

	_Value	GetX() const {
		return	m_v[0];
	}
	_Value	GetY() const {
		return	 m_v[1];
	}

	void	Set(int nIndex_, const _Value &v_ ) {
		m_v[nIndex_] = v_;
	}

	void	SetX( const _Value &v_ ) {
		m_v[0] = v_;
	}
	void	SetY( const _Value &v_ ) {
		m_v[1] = v_;
	}

	void	Normalize() {
		double dSum = hypot( m_v[0], m_v[1] );
		if ( dSum == 0 )
			return;

		double dMul = 1.0 / dSum;
		m_v[0] *= dMul;
		m_v[1] *= dMul;
	}

	friend Point_2T<_Value>		operator+( const Point_2T<_Value> &p1, const Point_2T<_Value> &p2 ) {
		return Point_2T<_Value>( p1.GetX() + p2.GetX(), p1.GetY() + p2.GetY() );
	}

	friend Point_2T<_Value>		operator-( const Point_2T<_Value> &p1, const Point_2T<_Value> &p2 ) {
		return Point_2T<_Value>( p1.GetX() - p2.GetX(), p1.GetY() - p2.GetY() );
	}

	friend Point_2T<_Value>		operator*( const Point_2T<_Value> &p1, const _Value &v_ ) {
		return Point_2T<_Value>( p1.GetX() * v_, p1.GetY() * v_ );
	}


private:
	_Value		m_v[2];
};

typedef GeoLib::Point_2T<double>	Point_2;

};	// namespace GeoLib
