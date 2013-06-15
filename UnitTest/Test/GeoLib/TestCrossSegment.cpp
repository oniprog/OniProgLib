///////////////////////////////////////////////////////////////////////////////////
/** @file
    @brief	線分同士の交差と近傍点の同一化のテスト
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

#include "GeoLib/Point_2.h"
#include "GeoLib/CrossSegment.h"

typedef GeoLib::Point_2   Point_2;

///////////////////////////////////////////////////////////////////////////////////
TEST(GetCrossLine_2, Test1 ) {

    Point_2 poiCross;
    ASSERT_TRUE( GeoLib::GetCrossLine_2(poiCross, Point_2(0,0), Point_2(10,10), Point_2(2,5), Point_2(10,5) ) );

    EXPECT_NEAR( 5.0, GeoLib::get<0>(poiCross), 1E-8 );
    EXPECT_NEAR( 5.0, GeoLib::get<1>(poiCross), 1E-8 );
}

///////////////////////////////////////////////////////////////////////////////////
TEST(CrossSegment_2, Test1) {

    GeoLib::CrossSegment_2 cs(1E-3);
    std::vector<Point_2>    listEdge;
    listEdge.push_back( Point_2(1E-7,1E-6) );
    listEdge.push_back( Point_2(1E-7,1E-6) );
    listEdge.push_back( Point_2(1e-8, 1e-8) );
    listEdge.push_back( Point_2(1e-8, 1e-8) );

    cs.Apply(listEdge.begin(), listEdge.end());

    ASSERT_EQ( 2, cs.size() );
}

///////////////////////////////////////////////////////////////////////////////////
TEST(CrossSegment_2, Test2) {

    GeoLib::CrossSegment_2 cs(1E-3);
    std::vector<Point_2>    listEdge;
    listEdge.push_back( Point_2(0, 0) );
    listEdge.push_back( Point_2(10, 10) );
    listEdge.push_back( Point_2(10, 0) );
    listEdge.push_back( Point_2(0, 10) );

    cs.Apply(listEdge.begin(), listEdge.end());

    ASSERT_EQ( 18, cs.size() );     // アルゴリズムに変更があったら引っかかるだろう．．．
}
