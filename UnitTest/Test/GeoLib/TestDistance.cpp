///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	距離計算のテスト
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

#include "GeoLib/Point_2.h"
#include "GeoLib/Distance.h"

typedef GeoLib::Point_2 Point_2;

///////////////////////////////////////////////////////////////////////////////////
TEST( GetDistancePP, Test1 ) {

    double dDist = GeoLib::GetDistancePP_2( Point_2(1,1), Point_2(10,2) );
    EXPECT_NEAR( std::sqrt(81.0+1.0), dDist, 1E-8 );
}

///////////////////////////////////////////////////////////////////////////////////
TEST( GetDistancePL, Test1 ) {

    double dDist = GeoLib::GetDistancePL_2( Point_2(1,1), Point_2(2,2), Point_2(0,0) );
    EXPECT_NEAR( 0.0, dDist, 1E-8 );
}

///////////////////////////////////////////////////////////////////////////////////
TEST( GetDistancePL, Test2 ) {

    double dDist = GeoLib::GetDistancePL_2( Point_2(1,1), Point_2(2,2), Point_2(1,0) );
    EXPECT_NEAR( std::sqrt(2.0)/2, dDist, 1E-8 );
}

///////////////////////////////////////////////////////////////////////////////////
TEST( GetDistancePL, Test3 ) {

    double dDist = GeoLib::GetDistancePL_2( Point_2(1,1), Point_2(1,10), Point_2(3,2) );
    EXPECT_NEAR( 2.0, dDist, 1E-8 );
}

///////////////////////////////////////////////////////////////////////////////////
TEST( GetDistancePL, Test4 ) {

    double dDist = GeoLib::GetDistancePL_2( Point_2(1,1), Point_2(10,1), Point_2(3,2) );
    EXPECT_NEAR( 1.0, dDist, 1E-8 );
}
