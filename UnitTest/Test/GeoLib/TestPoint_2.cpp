///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	Point_2のテスト
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

#include "GeoLib/Point_2.h"

TEST( Point_2, TestPlus ) {

	GeoLib::Point_2		p1(0.0,1.1);
	GeoLib::Point_2		p2(2.2,3.3);

	auto p3 = p1 + p2;
	EXPECT_NEAR( 2.2, p3.GetX(), 1E-8 );
	EXPECT_NEAR( 4.4, p3.GetY(), 1E-8 );
}

TEST( Point_2, TestMinus ) {

	GeoLib::Point_2		p1(0.0,1.1);
	GeoLib::Point_2		p2(2.2,3.3);

	auto p3 = p1 - p2;
	EXPECT_NEAR( -2.2, p3.GetX(), 1E-8 );
	EXPECT_NEAR( -2.2, p3.GetY(), 1E-8 );
}

TEST( Point_2, TestMul ) {

	GeoLib::Point_2		p1(0.0,1.1);
	GeoLib::Point_2		p2(2.2,3.3);

	auto p3 = p1 * 2;
	auto p4 = p2 * 3;

	EXPECT_NEAR( 0.0, p3.GetX(), 1E-8 );
	EXPECT_NEAR( 2.2, p3.GetY(), 1E-8 );

	EXPECT_NEAR( 6.6, p4.GetX(), 1E-8 );
	EXPECT_NEAR( 9.9, p4.GetY(), 1E-8 );
}
