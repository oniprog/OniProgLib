///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	三角形分割手法のテスト
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

#include "GeoLib/Delaunay.h"
#include "GeoLib/Triangulation2D.h"

#include <Windows.h>

typedef GeoLib::Point_2     Point_2;

///////////////////////////////////////////////////////////////////////////////////
TEST( TestTriangulation2D, Test1 ) {

    boost::mt19937 mt19937(0);
    boost::uniform_real<> real(0,1);
    auto rand = boost::variate_generator<boost::mt19937&, boost::uniform_real<>>( mt19937, real );

    GeoLib::Triangulation2D triangulation;
//		GeoLib::Delaunay2D<> triangulation;

    std::vector<Point_2> listPoint;

    DWORD nBeginTime = GetTickCount();

    for( int it=0; it<100; ++it ) {

        listPoint.push_back( Point_2( rand()*1000, rand()*1000 ) );
    }

    triangulation.Apply( listPoint.begin(), listPoint.end() );

    DWORD nEndTime = GetTickCount();
    printf("elipsed time : %d\n", nEndTime-nBeginTime );

#if 0
    {
        FILE *wfp = fopen("c:\\tmp\\a.tin", "w");
        auto ii = triangulation.begin(), ii_end = triangulation.end();
        for( ; ii != ii_end; ++ii) {

            auto poi = listPoint[*ii];
            fprintf(wfp, "%lf,%lf,200\n", GeoLib::get<0>(poi), GeoLib::get<1>(poi) );
        }

        fclose(wfp);
    }
#endif
}