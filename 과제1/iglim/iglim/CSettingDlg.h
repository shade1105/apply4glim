#pragma once
#include "afxdialogex.h"


// CSettingDlg 대화 상자

class CSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSettingDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CSettingDlg };
#endif



public:
	CString m_strDotRadSize;
	CString m_strThickness;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
