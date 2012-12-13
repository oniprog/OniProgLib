///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	Point_3のテスト
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

#include "GeoLib/Point_3.h"

TEST( Point_3, TestPlus ) {

	GeoLib::Point_3		p1(0.0,1.1, 2.2);
	GeoLib::Point_3		p2(2.2,3.3, 4.4);

	auto p3 = p1 + p2;
	EXPECT_NEAR( 2.2, p3.GetX(), 1E-8 );
	EXPECT_NEAR( 4.4, p3.GetY(), 1E-8 );
	EXPECT_NEAR( 6.6, p3.GetZ(), 1E-8 );
}

TEST( Point_3, TestMinus ) {

	GeoLib::Point_3		p1(0.0,1.1, 2.2);
	GeoLib::Point_3		p2(2.2,3.3, 4.4);

	auto p3 = p1 - p2;
	EXPECT_NEAR( -2.2, p3.GetX(), 1E-8 );
	EXPECT_NEAR( -2.2, p3.GetY(), 1E-8 );
	EXPECT_NEAR( -2.2, p3.GetZ(), 1E-8 );
}

TEST( Point_3, TestMul ) {

	GeoLib::Point_3		p1(0.0,1.1,2.2);
	GeoLib::Point_3		p2(2.2,3.3,4.4);

	auto p3 = p1 * 2;
	auto p4 = p2 * 3;

	EXPECT_NEAR( 0.0, p3.GetX(), 1E-8 );
	EXPECT_NEAR( 2.2, p3.GetY(), 1E-8 );
	EXPECT_NEAR( 4.4, p3.GetZ(), 1E-8 );

	EXPECT_NEAR( 6.6, p4.GetX(), 1E-8 );
	EXPECT_NEAR( 9.9, p4.GetY(), 1E-8 );
	EXPECT_NEAR( 13.2, p4.GetZ(), 1E-8 );
}

TEST( Point_3, TestSetGet ) {

	GeoLib::Point_3 p1;
	p1.SetX( 1.0 );
	p1.SetY( 2.0 );
	p1.SetZ( 3.0 );

	EXPECT_EQ( 1.0, p1.GetX() );
	EXPECT_EQ( 2.0, p1.GetY() );
	EXPECT_EQ( 3.0, p1.GetZ() );
}

TEST( Point_3, TestSetGet2 ) {

	GeoLib::Point_3 p1;
	p1.Set( 0, 1.0 );
	p1.Set( 1, 2.0 );
	p1.Set( 2, 3.0 );

	EXPECT_EQ( 1.0, p1.Get(0) );
	EXPECT_EQ( 2.0, p1.Get(1) );
	EXPECT_EQ( 3.0, p1.Get(2) );
}

TEST( Point_3, TestNormalize ) {

	GeoLib::Point_3 p1(1.0, 2.0, 3.0);
	p1.Normalize();

	EXPECT_NEAR( 1.0, p1.GetX()*p1.GetX() + p1.GetY()*p1.GetY() + p1.GetZ()*p1.GetZ(), 1E-8 );

	GeoLib::Point_3 p2(0.0, 0.0, 1.0);

	EXPECT_NEAR( 0.0, p2.GetX(), 1E-8 );
	EXPECT_NEAR( 0.0, p2.GetY(), 1E-8 );
	EXPECT_NEAR( 1.0, p2.GetZ(), 1E-8 );
}