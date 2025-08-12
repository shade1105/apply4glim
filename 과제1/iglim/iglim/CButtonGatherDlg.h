#pragma once
#include "afxdialogex.h"


// CButtonGatherDlg 대화 상자

class CButtonGatherDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CButtonGatherDlg)

public:
	CButtonGatherDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CButtonGatherDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CButtonGatherDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSetting();
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnRandom();
	afx_msg void OnBnClickedBtnRepeat();
};
