///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	数学関係のテスト
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

#include "GeoLib/Math.h"
#include "MPIR/mpirxx.h"

TEST( Determinant2x2, Test) {

	double dAns = GeoLib::Determinant2x2( 1, 2, 3, 4 );
	EXPECT_NEAR( -2, dAns, 1E-8 );
}

TEST( Determinant2x2, TestMPZ) {

	typedef mpz_class Z;
	double dAns = GeoLib::Determinant2x2( Z(1), Z(2), Z(3), Z(4) ).get_d();
	EXPECT_NEAR( -2, dAns, 1E-8 );
}

TEST( Determinant3x3, Test1 ) {

	double dAns = GeoLib::Determinant3x3(1, 0, 0, 0, 1, 2, 0, 3, 4);
	EXPECT_NEAR( -2, dAns, 1E-8 );

	double dAns2 = GeoLib::Determinant3x3( 0, 1, 0, 1, 0, 2, 3, 0, 4);
	EXPECT_NEAR( 2, dAns2, 1E-8 );

	double dAns3 = GeoLib::Determinant3x3( 0, 0, 1, 1, 2, 0, 3, 4, 0 );
	EXPECT_NEAR( -2, dAns3, 1E-8 );
}

TEST( Determinant3x3, Test2 ) {

	double dAns2 = GeoLib::Determinant3x3( 1, 0, 2, 0, 1, 0, 3, 0, 4);
	EXPECT_NEAR( -2, dAns2, 1E-8 );

	double dAns3 = GeoLib::Determinant3x3( 0, 1, 2, 1, 0, 0, 0, 3, 4 );
	EXPECT_NEAR( 2, dAns3, 1E-8 );

	double dAns4 = GeoLib::Determinant3x3( 0, 1, 2, 0, 3, 4, 1, 0, 0 );
	EXPECT_NEAR( -2, dAns4, 1E-8 );
}

TEST( Determinant3x3, Test3 ) {

	double dAns = GeoLib::Determinant3x3(1,1,1, 1,1,1, 1,1,1);
	EXPECT_NEAR(0.0, dAns, 1E-8 );
}
