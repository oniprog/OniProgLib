﻿///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	ライブラリのユニットテスト
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

/************************************************************************/
/** Debugモードでの実行はサポートしていません！ */
/************************************************************************/

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}