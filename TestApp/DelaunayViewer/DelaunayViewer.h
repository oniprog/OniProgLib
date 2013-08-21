
// DelaunayViewer.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CDelaunayViewerApp:
// このクラスの実装については、DelaunayViewer.cpp を参照してください。
//

class CDelaunayViewerApp : public CWinApp
{
public:
	CDelaunayViewerApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CDelaunayViewerApp theApp;