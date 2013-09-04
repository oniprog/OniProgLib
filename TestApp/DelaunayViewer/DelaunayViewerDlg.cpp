
// DelaunayViewerDlg.cpp : 実装ファイル
//

#include "stdafx.h"

#include <set>
#include <limits>

#undef min
#undef max

#include "DelaunayViewer.h"
#include "DelaunayViewerDlg.h"
#include "afxdialogex.h"

#include "GeoLib/Triangulation2D.h"
#include "GeoLib/Delaunay.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef GeoLib::Point_2     Point_2;

std::vector<Point_2>        g_listPoint;
std::vector<Point_2>        g_listTris;


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDelaunayViewerDlg ダイアログ



CDelaunayViewerDlg::CDelaunayViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDelaunayViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDelaunayViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDelaunayViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDBLCLK()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDelaunayViewerDlg メッセージ ハンドラー

BOOL CDelaunayViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CDelaunayViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CDelaunayViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
	//	CDialogEx::OnPaint();
        CPaintDC dc(this);

        CRect rectClient;
        GetClientRect(&rectClient);

        dc.FillRect( &rectClient, &CBrush(RGB(0,0,0)));

        CPen pen1(PS_SOLID, 1, RGB(0xff,0xff,0xff));
        auto pOldPen = dc.SelectObject( &pen1 );

        auto ii = g_listTris.cbegin(), ii_end = g_listTris.cend();
        for(; ii != ii_end; ) {

            auto pt1 = *ii;     ++ii;
            auto pt2 = *ii;     ++ii;
            auto pt3 = *ii;     ++ii;

            dc.MoveTo( (int)GeoLib::get<0>(pt1), (int)GeoLib::get<1>(pt1) );
            dc.LineTo( (int)GeoLib::get<0>(pt2), (int)GeoLib::get<1>(pt2) );
            dc.LineTo( (int)GeoLib::get<0>(pt3), (int)GeoLib::get<1>(pt3) );
            dc.LineTo( (int)GeoLib::get<0>(pt1), (int)GeoLib::get<1>(pt1) );
        }

        CPen pen2( PS_SOLID, 2, RGB(0xff,0x30,0x30));
        dc.SelectObject( &pen2 );

        auto jj = g_listPoint.cbegin(), jj_end = g_listPoint.cend();
        for( ; jj != jj_end; ++jj ) {

            int ix = static_cast<int>( GeoLib::get<0>(*jj) );
            int iy = static_cast<int>( GeoLib::get<1>(*jj) );
            const int nSize = 3;

            dc.MoveTo(ix-nSize, iy-nSize);
            dc.LineTo(ix+nSize+1, iy+nSize+1);
            dc.MoveTo(ix+nSize, iy-nSize);
            dc.LineTo(ix-nSize+1, iy+nSize+1);
        }

        dc.SelectObject( pOldPen );
    }
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CDelaunayViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDelaunayViewerDlg::CalcDelaunay() {

    g_listTris.clear();

    if ( g_listPoint.size() < 3 )
        return;
    
#if 0
    // ヒューリスティクス手法による分割
    GeoLib::Triangulation2D triangulation;
    triangulation.Apply( g_listPoint.begin(), g_listPoint.end() );

    auto ii = triangulation.cbegin(), ii_end = triangulation.cend();
    for( ; ii != ii_end; ++ii ) {
        g_listTris.push_back( g_listPoint[*ii] );
    }

#else
    // ドロネー分割手法による分割
    GeoLib::Delaunay2D<> delaunay;
    delaunay.Apply( g_listPoint.begin(), g_listPoint.end() );

    auto ii = delaunay.cbegin(), ii_end = delaunay.cend();
    for( ; ii != ii_end; ++ii ) {
        g_listTris.push_back( g_listPoint[*ii] );
    }
#endif
}

void CDelaunayViewerDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    g_listPoint.push_back( Point_2(point.x, point.y));
    CalcDelaunay();
    Invalidate();

    CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CDelaunayViewerDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
{
    double dMinDist = std::numeric_limits<double>::max();

    int iFind = -1;
    for( int it=0; it<(int)g_listPoint.size(); ++it ) {

        auto poi = g_listPoint[it];
        double dDist = 
            (GeoLib::get<0>(poi)-point.x)*(GeoLib::get<0>(poi)-point.x) +
            (GeoLib::get<1>(poi)-point.y)*(GeoLib::get<1>(poi)-point.y);
        if ( dDist < dMinDist ) {

            dMinDist = dDist;
            iFind = it;
        }
    }

    if (iFind >= 0 ) {

        g_listPoint.erase( g_listPoint.begin() + iFind );
        CalcDelaunay();
    }

    Invalidate();

    CDialogEx::OnRButtonDblClk(nFlags, point);
}


BOOL CDelaunayViewerDlg::OnEraseBkgnd(CDC* pDC)
{
//    return CDialogEx::OnEraseBkgnd(pDC);
    return TRUE;
}
