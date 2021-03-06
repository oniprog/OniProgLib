﻿///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	数学関係のテスト
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

#include "GeoLib/Delaunay.h"
#include "GeoLib/Rational.h"

#include "boost/random.hpp"

typedef GeoLib::Point_2 Point_2;

///////////////////////////////////////////////////////////////////////////////////
TEST( Delaunay2D, Test2 ) {

    Point_2 pt1(0,0), pt2(1,0), pt3(0,1);
    Point_2 pt4(0.1, 0.1);


    // 左回りの三角形の中に点があると正の数となる
    double dInCircle1 = GeoLib::InCircleExact( pt1, pt2, pt3, pt4 );
    EXPECT_TRUE( dInCircle1 > 0 );

    double dInCircle2 = GeoLib::InCircleExact( pt1, pt3, pt2, pt4 );
    EXPECT_TRUE( dInCircle2 < 0 );
}

///////////////////////////////////////////////////////////////////////////////////
TEST( Delaunay2D, Test1 ) {

    GeoLib::Delaunay2D<> delaunay;
    std::vector<Point_2> listPoint;

    boost::mt19937 mt19937(0);
    boost::uniform_real<> real(0,1);
    auto rand = boost::variate_generator<boost::mt19937&, boost::uniform_real<>>( mt19937, real );

    for( int it=0; it<20; ++it ) {

        listPoint.push_back( Point_2(rand()*1000, rand()*1000) );
    }

    delaunay.Apply(listPoint.begin(), listPoint.end() );

#if 0 // 出力する．MakeJibanやGodai 3D Readerで読める
    FILE *wfp = fopen("c:\\tmp\\a.tin", "w");
    auto ii = delaunay.cbegin(), ii_end = delaunay.cend();
    for(; ii != ii_end; ) {

        auto i1 = *ii;
        auto pt1 = listPoint[i1];      ++ii;
        fprintf(wfp, "%lf,%lf,%lf\n", GeoLib::get<0>(pt1), GeoLib::get<1>(pt1), 1000.0 );
    }

    fclose(wfp);
#endif
}