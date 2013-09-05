
// DelaunayViewerDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxwin.h"


// CDelaunayViewerDlg ダイアログ
class CDelaunayViewerDlg : public CDialogEx
{
// コンストラクション
public:
	CDelaunayViewerDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_DELAUNAYVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


private:
    void        CalcDelaunay();

// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedRadDelaunay();
	afx_msg void OnBnClickedRadOriginal();
	CButton m_RAD_Original;
	CButton m_RAD_Delaunay;
};
