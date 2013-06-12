///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	3次元点クラス
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Point_2.h"

namespace GeoLib {

template<class _Value>
class Point_3T {
public:
	typedef _Value	value_type;

public:
	Point_3T() {}	// 高速化のために未初期化
	Point_3T( const _Value &v1_, const _Value &v2_, const _Value &v3_ ) {
		Define( v1_, v2_, v3_ );
	}

	void	Define( const _Value &v1_, const _Value &v2_ , const _Value &v3_) {
		m_v[0] = v1_;	m_v[1] = v2_;	m_v[2] = v3_;
	}

	_Value	Get(int nIndex) const {
		return m_v[nIndex];
	}

	_Value	GetX() const {
		return	m_v[0];
	}
	_Value	GetY() const {
		return	m_v[1];
	}
	_Value	GetZ() const {
		return	m_v[2];
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
	void	SetZ( const _Value &v_ ) {
		m_v[2] = v_;
	}

	void	Normalize() {
		double dSum = std::sqrt( m_v[0]*m_v[0] + m_v[1]*m_v[1] + m_v[2]*m_v[2] );
		if ( dSum == 0 )
			return;

		double dMul = 1.0 / dSum;
		m_v[0] *= dMul;
		m_v[1] *= dMul;
		m_v[2] *= dMul;
	}

	friend Point_3T<_Value>		operator-( const Point_3T<_Value> &p1 ) {
		return Point_3T<_Value>( -p1.GetX(), -p1.GetY(), -p1.GetZ() );
	}

	friend Point_3T<_Value>		operator+( const Point_3T<_Value> &p1, const Point_3T<_Value> &p2 ) {
		return Point_3T<_Value>( p1.GetX() + p2.GetX(), p1.GetY() + p2.GetY(), p1.GetZ() + p2.GetZ() );
	}

	friend Point_3T<_Value>		operator-( const Point_3T<_Value> &p1, const Point_3T<_Value> &p2 ) {
		return Point_3T<_Value>( p1.GetX() - p2.GetX(), p1.GetY() - p2.GetY(), p1.GetZ() - p2.GetZ() );
	}

	friend Point_3T<_Value>		operator*( const Point_3T<_Value> &p1, const _Value &v_ ) {
		return Point_3T<_Value>( p1.GetX() * v_, p1.GetY() * v_, p1.GetZ() * v_ );
	}


private:
	_Value		m_v[3];
};

typedef GeoLib::Point_3T<double>	Point_3;

///////////////////////////////////////////////////////////////////////////////////////////////
namespace traits {

template<>
struct access<Point_3,0> {
	static Point_3::value_type get(Point_3 const & p ) {
		return p.GetX();
	}
};
template<>
struct access<Point_3,1> {
	static Point_3::value_type get(Point_3 const & p ) {
		return p.GetY();
	}
};
template<>
struct access<Point_3,2> {
	static Point_3::value_type get(Point_3 const & p ) {
		return p.GetZ();
	}
};

};  // namespace traits

};	// namespace GeoLib
