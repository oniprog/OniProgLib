///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	数学関係のテスト
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

#include "boost/multiprecision/gmp.hpp"
#include <limits>

#include "GeoLib/Math.h"
//#include "MPIR/mpirxx.h"

#include "GeoLib/Point_2.h"
#include "GeoLib/Point_3.h"
#include "GeoLib/Rational.h"

///////////////////////////////////////////////////////////////////////////////////
TEST( Determinant2x2, Test) {

	double dAns = GeoLib::Determinant2x2( 1, 2, 3, 4 );
	EXPECT_NEAR( -2, dAns, 1E-8 );
}

///////////////////////////////////////////////////////////////////////////////////
TEST( Determinant2x2, TestMPZ) {

	GeoLib::Rational a(1.0), b(2.0),c(3.0),d(4.0);
    double dAns = GeoLib::static_rational_cast<double>( GeoLib::Determinant2x2( a, b, c, d ) );
	EXPECT_NEAR( -2, dAns, 1E-8 );
}

///////////////////////////////////////////////////////////////////////////////////
TEST( Determinant3x3, Test1 ) {

	double dAns = GeoLib::Determinant3x3(1, 0, 0, 0, 1, 2, 0, 3, 4);
	EXPECT_NEAR( -2, dAns, 1E-8 );

	double dAns2 = GeoLib::Determinant3x3( 0, 1, 0, 1, 0, 2, 3, 0, 4);
	EXPECT_NEAR( 2, dAns2, 1E-8 );

	double dAns3 = GeoLib::Determinant3x3( 0, 0, 1, 1, 2, 0, 3, 4, 0 );
	EXPECT_NEAR( -2, dAns3, 1E-8 );
}

///////////////////////////////////////////////////////////////////////////////////
TEST( Determinant3x3, Test2 ) {

	double dAns2 = GeoLib::Determinant3x3( 1, 0, 2, 0, 1, 0, 3, 0, 4);
	EXPECT_NEAR( -2, dAns2, 1E-8 );

	double dAns3 = GeoLib::Determinant3x3( 0, 1, 2, 1, 0, 0, 0, 3, 4 );
	EXPECT_NEAR( 2, dAns3, 1E-8 );

	double dAns4 = GeoLib::Determinant3x3( 0, 1, 2, 0, 3, 4, 1, 0, 0 );
	EXPECT_NEAR( -2, dAns4, 1E-8 );
}

///////////////////////////////////////////////////////////////////////////////////
TEST( Determinant3x3, Test3 ) {

	double dAns = GeoLib::Determinant3x3(1,1,1, 1,1,1, 1,1,1);
	EXPECT_NEAR(0.0, dAns, 1E-8 );
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
TEST( Orient_2, Test1 ) {

	GeoLib::Point_2 p1( 0, 0 ), p2(0, 1), p3(1,1), p4(-1, 1), p5(0,2);

	double dRight = GeoLib::Orient_2( p1, p2, p3 );
	EXPECT_TRUE( dRight < 0 );

	double dLeft = GeoLib::Orient_2( p1, p2, p4 );
	EXPECT_TRUE( dLeft > 0 );

	double dZero = GeoLib::Orient_2( p1, p2, p5 );
	EXPECT_NEAR( 0.0, dZero, 1E-30 );

	double dInvRight = GeoLib::Orient_2( p1, p3, p2 );	// swap p2, p3
	EXPECT_TRUE( dInvRight > 0 );

	double dInvLeft = GeoLib::Orient_2( p1, p4, p2 );	// swap p2, p4
	EXPECT_TRUE( dInvLeft < 0 );
}


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
TEST( OrientExact_2, Test1 ) {

	GeoLib::Point_2 p1( 0, 0 ), p2(0, 1), p3(1,1), p4(-1, 1), p5(0,2);

	double dRight = GeoLib::OrientExact_2( p1, p2, p3 );
	EXPECT_TRUE( dRight < 0 );

	double dLeft = GeoLib::OrientExact_2( p1, p2, p4 );
	EXPECT_TRUE( dLeft > 0 );

	double dZero = GeoLib::OrientExact_2( p1, p2, p5 );
	EXPECT_NEAR( 0.0, dZero, 1E-30 );

	double dInvRight = GeoLib::OrientExact_2( p1, p3, p2 );	// swap p2, p3
	EXPECT_TRUE( dInvRight > 0 );

	double dInvLeft = GeoLib::OrientExact_2( p1, p4, p2 );	// swap p2, p4
	EXPECT_TRUE( dInvLeft < 0 );
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
TEST( Orient_3, Test1 ) {

	GeoLib::Point_3 p1(0,0,0), p2(1,0,0), p3(0,1,0),  p4(0,0,1), p5(0,0,-1), p6(2,2,0);

	double dRight = GeoLib::Orient_3( p1, p2, p3, p4 );
	EXPECT_TRUE( dRight < 0 );

	double dLeft = GeoLib::Orient_3( p1, p2, p3, p5 );
	EXPECT_TRUE( dLeft > 0 );

	double dZero = GeoLib::Orient_3( p1, p2, p3, p6 );
	EXPECT_NEAR( 0.0, dZero, 1E-30 );

	double dInvRight = GeoLib::Orient_3( p1, p2, p4, p3 );	// swap p3, p4
	EXPECT_TRUE( dInvRight > 0 );

	double dInvLeft = GeoLib::Orient_3( p1, p2, p5, p3 );	// swap p3, p5
	EXPECT_TRUE( dInvLeft < 0 );
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
TEST( Orient_Exact3, Test1 ) {

	GeoLib::Point_3 p1(0,0,0), p2(1,0,0), p3(0,1,0),  p4(0,0,1), p5(0,0,-1), p6(2,2,0);

	double dRight = GeoLib::OrientExact_3( p1, p2, p3, p4 );
	EXPECT_TRUE( dRight < 0 );

	double dLeft = GeoLib::OrientExact_3( p1, p2, p3, p5 );
	EXPECT_TRUE( dLeft > 0 );

	double dZero = GeoLib::OrientExact_3( p1, p2, p3, p6 );
	EXPECT_NEAR( 0.0, dZero, 1E-30 );

	double dInvRight = GeoLib::OrientExact_3( p1, p2, p4, p3 );	// swap p3, p4
	EXPECT_TRUE( dInvRight > 0 );

	double dInvLeft = GeoLib::OrientExact_3( p1, p2, p5, p3 );	// swap p3, p5
	EXPECT_TRUE( dInvLeft < 0 );
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
TEST( InCircle, Test1 ) {

	GeoLib::Point_2 p1(-1,0), p2(1,0), p3(0,1);

	double dIn1 = GeoLib::InCircle( p1, p2, p3, GeoLib::Point_2(0,0));
	EXPECT_TRUE( dIn1 > 0 );

	double dIn2 = GeoLib::InCircle( p1, p2, p3, GeoLib::Point_2(2,0));
	EXPECT_TRUE( dIn2 < 0 );

	double dIn3 = GeoLib::InCircle( p2, p1, p3, GeoLib::Point_2(0,0));
	EXPECT_TRUE( dIn3 < 0 );

	double dIn4 = GeoLib::InCircle( p2, p1, p3, GeoLib::Point_2(2,0));
	EXPECT_TRUE( dIn4 > 0 );

	double dIn5 = GeoLib::InCircle( p1, p2, p3, GeoLib::Point_2(0,-1));
	EXPECT_NEAR( 0, dIn5, 1E-8 );
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
TEST( InCircleExact, Test1 ) {

	GeoLib::Point_2 p1(-1,0), p2(1,0), p3(0,1);

	double dIn1 = GeoLib::InCircleExact( p1, p2, p3, GeoLib::Point_2(0,0));
	EXPECT_TRUE( dIn1 > 0 );

	double dIn2 = GeoLib::InCircleExact( p1, p2, p3, GeoLib::Point_2(2,0));
	EXPECT_TRUE( dIn2 < 0 );

	double dIn3 = GeoLib::InCircleExact( p2, p1, p3, GeoLib::Point_2(0,0));
	EXPECT_TRUE( dIn3 < 0 );

	double dIn4 = GeoLib::InCircleExact( p2, p1, p3, GeoLib::Point_2(2,0));
	EXPECT_TRUE( dIn4 > 0 );

	double dIn5 = GeoLib::InCircleExact( p1, p2, p3, GeoLib::Point_2(0,-1));
	EXPECT_NEAR( 0, dIn5, 1E-8 );
}