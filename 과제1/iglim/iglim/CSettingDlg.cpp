// CSettingDlg.cpp: 구현 파일
//

#include "pch.h"
#include "iglim.h"
#include "afxdialogex.h"
#include "CSettingDlg.h"


// CSettingDlg 대화 상자

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CSettingDlg, pParent)
{
	m_strDotRadSize = _T("1");	//점 크기
	m_strThickness = _T("1");	//원 두께
}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DOTRADSIZE, m_strDotRadSize);
	DDX_Text(pDX, IDC_EDIT_THICKNESS, m_strThickness);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSettingDlg 메시지 처리기

void CSettingDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	CDialogEx::OnOK();
}
