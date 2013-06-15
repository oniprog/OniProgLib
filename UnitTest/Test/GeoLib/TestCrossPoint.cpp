///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	交点計算のテスト
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

#include "GeoLib/Point_2.h"
#include "GeoLib/CrossPoint.h"

typedef GeoLib::Point_2 Point_2;

///////////////////////////////////////////////////////////////////////////////////
TEST( IsCrossSegment_2, Test1 ) {

    bool bCross1 = GeoLib::IsCrossSegment_2( Point_2(0,0), Point_2(0,10), Point_2(-2,4), Point_2(-0.1,4) );
    EXPECT_FALSE( bCross1 );

    bool bCross2 = GeoLib::IsCrossSegment_2( Point_2(0,0), Point_2(0,10), Point_2(-2,4), Point_2(0,4) );
    EXPECT_TRUE( bCross2 );

    bool bCross3 = GeoLib::IsCrossSegment_2( Point_2(0,0), Point_2(0,10), Point_2(-2,4), Point_2(4,4) );
    EXPECT_TRUE( bCross2 );
}
///////////////////////////////////////////////////////////////////////////////////
TEST(GetCrossLine_2, Test1 ) {

    Point_2 poiCross;
    ASSERT_TRUE( GeoLib::GetCrossLine_2(poiCross, Point_2(0,0), Point_2(10,10), Point_2(2,5), Point_2(10,5) ) );

    EXPECT_NEAR( 5.0, GeoLib::get<0>(poiCross), 1E-8 );
    EXPECT_NEAR( 5.0, GeoLib::get<1>(poiCross), 1E-8 );
}

