///////////////////////////////////////////////////////////////////////////////////
/** @file
    @brief	無誤差演算のテスト
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

#include "ExactDouble/ExactDoubleFast.h"

///////////////////////////////////////////////////////////////////////////////////
#if 0
#include "GeoLib/Rational.h"
TEST( TestVsBoost, Test1 ) {

    const int N = 1000000;

#if 1
    GeoLib::Rational a(1E-300), b(1E300), c;
    for( int it=0; it<N; ++it ) {
        c = a + b;
    }
    std::cout << GeoLib::static_rational_cast<double>(c) << std::endl;
#else
    double v;
    ExactDouble::EF a(1E-300), b(1E300);
    ExactDouble::ExactDoubleFast<2> c;

    for( int it=0; it<N ; ++it ) {
        c = a + b;
    }
    std::cout << *c.begin() << std::endl;
#endif
}

#endif

///////////////////////////////////////////////////////////////////////////////////
TEST( TestExactDoubleFast, TestAdd1 ) {

    ExactDouble::EF v1(2), v2(1);
    auto v3 = v1 + v2;

    ASSERT_TRUE( v3.size() == 2 );
    auto ii = v3.cbegin();
    EXPECT_EQ( 0.0, *ii++);
    EXPECT_EQ( 3.0, *ii );
}

///////////////////////////////////////////////////////////////////////////////////
TEST( TestExactDoubleFast, TestAdd2 ) {

    ExactDouble::EF v1(1E100), v2(-1);
    auto v3 = v1 + v2;

    ASSERT_TRUE( v3.size() == 2 );
    auto ii = v3.cbegin();
    EXPECT_EQ( -1, *ii++);
    EXPECT_EQ( 1E100, *ii );
}

///////////////////////////////////////////////////////////////////////////////////
TEST( TestExactDoubleFast, TestSub1 ) {

    ExactDouble::EF v1(10), v2(20);
    auto v3 = v1 - v2;

    ASSERT_TRUE( v3.size() == 2 );
    auto ii = v3.cbegin();
    EXPECT_EQ( 0, *ii++);
    EXPECT_EQ( -10, *ii );
}

///////////////////////////////////////////////////////////////////////////////////
TEST( TestExactDoubleFast, TestMul1 ) {

    ExactDouble::EF v1(2);
    auto v3 = v1 * 3.5;

    ASSERT_TRUE( v3.size() == 2 );
    auto ii = v3.cbegin();
    EXPECT_EQ( 0.0, *ii++ );
    EXPECT_EQ( 7.0, *ii );
}