///////////////////////////////////////////////////////////////////////////////////
/** @file
    @brief	線分同士の交差と近傍点の同一化のテスト
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

#include "GeoLib/Point_2.h"
#include "GeoLib/FastCrossSegment.h"

typedef GeoLib::Point_2   Point_2;

///////////////////////////////////////////////////////////////////////////////////
TEST(FastCrossSegment_2, Test1) {

    GeoLib::FastCrossSegment_2 cs(1E-3);
    std::vector<Point_2>    listEdge;
    listEdge.push_back( Point_2(1E-7,1E-6) );
    listEdge.push_back( Point_2(1E-7,1E-6) );
    listEdge.push_back( Point_2(1e-8, 1e-8) );
    listEdge.push_back( Point_2(1e-8, 1e-8) );

    cs.Apply(listEdge.begin(), listEdge.end());

    ASSERT_EQ( 2, cs.size() );
}

///////////////////////////////////////////////////////////////////////////////////
TEST(FastCrossSegment_2, Test2) {

    GeoLib::FastCrossSegment_2 cs(1E-3);
    std::vector<Point_2>    listEdge;
    listEdge.push_back( Point_2(0, 0) );
    listEdge.push_back( Point_2(10, 10) );
    listEdge.push_back( Point_2(10, 0) );
    listEdge.push_back( Point_2(0, 10) );

    cs.Apply(listEdge.begin(), listEdge.end());

    ASSERT_EQ( 8, cs.size() );     // アルゴリズムに変更があったら引っかかるだろう．．．
}

#if 0
#include <Windows.h>
///////////////////////////////////////////////////////////////////////////////////
TEST(FastCrossSegment_2, Test3) {

    GeoLib::FastCrossSegment_2 cs(1E-3);
    std::vector<Point_2> listEdge;

    boost::mt19937 mt19937(0);
    boost::uniform_real<> real(0,1);
    auto rand = boost::variate_generator<boost::mt19937&, boost::uniform_real<>>( mt19937, real );

    for( int it=0; it<5000; ++it) {
        listEdge.push_back( Point_2(rand()*10000, rand()*10000 ) );
        listEdge.push_back( listEdge.back() + Point_2(rand()*100-50, rand()*100-50) );
    }

	DWORD nBeginTime = GetTickCount();
    cs.Apply( listEdge.begin(), listEdge.end() );
	DWORD nRTime1 = GetTickCount();

#if 0
	// チェック用
    GeoLib::FastCrossSegment_2 csBrute(1E-3, 1E-8, 10, 0);
	csBrute.Apply( listEdge.begin(), listEdge.end());
	DWORD nRTime2 = GetTickCount();

	DWORD nTime1 = nRTime1 - nBeginTime;
	DWORD nTime2 = nRTime2 - nRTime1;
	std::cout << "Speed up : "<<nTime2*1.00/nTime1 << std::endl;

	EXPECT_EQ( csBrute.size() , cs.size() );

#endif
#if 0
    FILE *wfp = fopen("c:\\tmp\\b.tin", "w");
    auto ii = cs.cbegin(), ii_end = cs.cend();
    for(; ii != ii_end; ) {

        double dR = rand();
        double dG = rand();
        double dB = rand();

        auto pe1 = **ii; ++ii;
        auto pe2 = **ii; ++ii;
		auto pn = GeoLib::Point_2( pe1.GetY()-pe2.GetY(), pe2.GetX() - pe1.GetX() );
		pn.Normalize();
		auto pq11 = pe1 + pn;
		auto pq12 = pe1 - pn;
		auto pq21 = pe2 + pn;
		auto pq22 = pe2 - pn;

        fprintf(wfp, "%lf,%lf,%lf,%lf,%lf,%lf,1.0\n", GeoLib::get<0>(pq11), GeoLib::get<1>(pq11), 10.0, dR ,dG, dB  );
        fprintf(wfp, "%lf,%lf,%lf,%lf,%lf,%lf,1.0\n", GeoLib::get<0>(pq12), GeoLib::get<1>(pq12), 10.0, dR ,dG, dB  );
        fprintf(wfp, "%lf,%lf,%lf,%lf,%lf,%lf,1.0\n", GeoLib::get<0>(pq21), GeoLib::get<1>(pq21), 10.0, dR ,dG, dB  );

        fprintf(wfp, "%lf,%lf,%lf,%lf,%lf,%lf,1.0\n", GeoLib::get<0>(pq21), GeoLib::get<1>(pq21), 10.0, dR ,dG, dB  );
        fprintf(wfp, "%lf,%lf,%lf,%lf,%lf,%lf,1.0\n", GeoLib::get<0>(pq12), GeoLib::get<1>(pq12), 10.0, dR ,dG, dB  );
        fprintf(wfp, "%lf,%lf,%lf,%lf,%lf,%lf,1.0\n", GeoLib::get<0>(pq22), GeoLib::get<1>(pq22), 10.0, dR ,dG, dB  );
    }
#endif
}
#endif
