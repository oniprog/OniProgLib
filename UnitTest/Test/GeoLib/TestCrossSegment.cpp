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

#if 0
///////////////////////////////////////////////////////////////////////////////////
TEST(CrossSegment_2, Test3) {

    GeoLib::CrossSegment_2 cs(1E-3);
    std::vector<Point_2> listEdge;

    boost::mt19937 mt19937(0);
    boost::uniform_real<> real(0,1);
    auto rand = boost::variate_generator<boost::mt19937&, boost::uniform_real<>>( mt19937, real );

    for( int it=0; it<200; ++it) {
        listEdge.push_back( Point_2(rand()*1000, rand()*1000 ) );
        listEdge.push_back( listEdge.back() + Point_2(rand()*400-200, rand()*400-200) );
    }

    cs.Apply( listEdge.begin(), listEdge.end() );

    FILE *wfp = fopen("c:\\tmp\\b.tin", "w");
    auto ii = cs.cbegin(), ii_end = cs.cend();
    for(; ii != ii_end; ) {

        double dR = rand();
        double dG = rand();
        double dB = rand();

        auto pe1 = **ii; ++ii;
        fprintf(wfp, "%lf,%lf,%lf,%lf,%lf,%lf,1.0\n", GeoLib::get<0>(pe1), GeoLib::get<1>(pe1), 10.0, dR ,dG, dB  );
        auto pe2 = **ii; ++ii;
        fprintf(wfp, "%lf,%lf,%lf,%lf,%lf,%lf,1.0\n", GeoLib::get<0>(pe2), GeoLib::get<1>(pe2), 10.0, dR ,dG, dB );
        fprintf(wfp, "%lf,%lf,%lf,%lf,%lf,%lf,1.0\n", GeoLib::get<0>(pe2), GeoLib::get<1>(pe2), 10.0, dR ,dG, dB );
    }
}
#endif
