
// iglimDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "iglim.h"
#include "iglimDlg.h"
#include "afxdialogex.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_MOUSEMOVE()
//	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CiglimDlg 대화 상자



CiglimDlg::CiglimDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IGLIM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nDotRadSize = 5;
	m_nThickness = 3;
}

void CiglimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CiglimDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SETTING, &CiglimDlg::OnBnClickedBtnSetting)
	ON_BN_CLICKED(IDC_BTN_RESET, &CiglimDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_RANDOM, &CiglimDlg::OnBnClickedBtnRandom)
	ON_BN_CLICKED(IDC_BTN_REPEAT, &CiglimDlg::OnBnClickedBtnRepeat)
	ON_MESSAGE(WM_THREAD_ACTIVE, &CiglimDlg::OnThreadActive)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CiglimDlg 메시지 처리기

BOOL CiglimDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	srand(time(0));
	GetWindowRect(&m_rect);
	m_bLeftClick = false;
	m_nBpp = 8;
	m_bThreadStatus = true;
	
	// 버튼의 위치와 크기 구하기
	CRect btnRect;
	GetDlgItem(IDC_BTN_REPEAT)->GetWindowRect(&btnRect);
	ScreenToClient(&btnRect);
	m_nImageWidth = m_rect.Width();
	m_nImageHeight = btnRect.top - m_rect.top - 10;

	m_Image.Create(m_nImageWidth, -m_nImageHeight, m_nBpp);
	
	SetClearImage();
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CiglimDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CiglimDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	DrawScreen((unsigned char*)m_Image.GetBits());
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CiglimDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//현재 픽셀 위치 확인
bool CiglimDlg::IsInCircle(CPoint Dot, CPoint Center, int nRadius)
{
	bool bRet = false;
	double dX = Dot.x - Center.x;
	double dY = Dot.y - Center.y;
	double dDist = (dX * dX) + (dY * dY);

	if (dDist < nRadius * nRadius)
		bRet = true;

	return bRet;
}

//원 이미지 생성
void CiglimDlg::MakeImageCircle(unsigned char* pszImage, CPoint Center, int nRadius, int nGray)
{
	int x = Center.x - nRadius;
	int y = Center.y - nRadius;
	int nPitch = m_Image.GetPitch();

	for (int j = y; j < y + nRadius * 2; j++) {
		for (int i = x; i < x + nRadius * 2; i++) {
			if (ValidImagePos(CPoint(i, j)) && IsInCircle(CPoint(i, j), Center, nRadius))
				pszImage[j * nPitch + i] = nGray;
		}
	}
}

//범위 난수 생성
int CiglimDlg::GetRandomNumber(int min, int max)
{
	return min + (rand() % (max - min + 1));
}

//3개 좌표로 그릴 원 중앙 계산
CPoint CiglimDlg::CalcCircleCenter(CPoint DotA, CPoint DotB, CPoint DotC)
{
	CPoint Center;
	double dA = DotA.x * (DotB.y - DotC.y) - DotA.y * (DotB.x - DotC.x) + DotB.x * DotC.y - DotC.x * DotB.y;
	double dB = (DotA.x * DotA.x + DotA.y * DotA.y) * (DotC.y - DotB.y) + (DotB.x * DotB.x + DotB.y * DotB.y) * (DotA.y - DotC.y) + (DotC.x * DotC.x + DotC.y * DotC.y) * (DotB.y - DotA.y);
	double dC = (DotA.x * DotA.x + DotA.y * DotA.y) * (DotB.x - DotC.x) + (DotB.x * DotB.x + DotB.y * DotB.y) * (DotC.x - DotA.x) + (DotC.x * DotC.x + DotC.y * DotC.y) * (DotA.x - DotB.x);

	Center.x = -dB / (2 * dA);
	Center.y = -dC / (2 * dA);

	return Center;
}

//원의 선 위에 있는지 확인
bool CiglimDlg::IsOnLine(CPoint Dot, CPoint Center, int nRadius)
{
	bool bRet = false;
	
	if (IsInCircle(Dot, Center, nRadius) && !IsInCircle(Dot, Center, nRadius - m_nThickness))
		bRet = true;

	return bRet;
}

//외곽선 있는 원 그리기
void CiglimDlg::MakeImageCircleOutline(unsigned char* pszImage, CPoint Center, int nRadius, int nGray)
{
	CClientDC dc(this);
	int x = Center.x - nRadius;
	int y = Center.y - nRadius;
	int nPitch = m_Image.GetPitch();

	for (int j = y; j < y + nRadius * 2; j++) {
		for (int i = x; i < x + nRadius * 2; i++) {
			if (ValidImagePos(CPoint(i, j)) && IsOnLine(CPoint(i, j), Center, nRadius))
				pszImage[j * nPitch + i] = nGray;
		}
	}
}

//멤버변수 점 3개 이용해서 그리기
void CiglimDlg::MakeImageCircleOutlineWithDots(unsigned char* pszImage)
{
	int nIntMin = -(1 << (sizeof(int) * 8 - 1));

	if (m_Dots.size() == 3) {
		CPoint Center = CalcCircleCenter(m_Dots[0], m_Dots[1], m_Dots[2]);
		int nRadius = sqrt((m_Dots[0].x - Center.x) * (m_Dots[0].x - Center.x) + (m_Dots[0].y - Center.y) * (m_Dots[0].y - Center.y));
		cout << Center.x << " " << Center.y << endl;

		//자료형 넘어가면 리턴
		if (Center.x <= nIntMin)
			return;
		MakeImageCircleOutline(pszImage, Center, nRadius);
	}
}

//점 그리기
void CiglimDlg::MakeImageDot(unsigned char* pszImage, CPoint Point)
{
	m_Dots.push_back(Point);
	MakeImageCircle(pszImage, Point, m_nDotRadSize);
}

//저장된 점 그리기
void CiglimDlg::MakeImageDots(unsigned char* pszImage)
{
	for (CPoint Dot : m_Dots) {
		MakeImageCircle(pszImage, Dot, m_nDotRadSize);
	}
}

//점 초기화
void CiglimDlg::RemoveDots()
{
	m_Dots.clear();
}

//랜덤 점 생성
void CiglimDlg::GenerateImageRandomDots(unsigned char* pszImage)
{
	RemoveDots();
	SetClearImage();
	for(int i = 0; i < 3; i++)
		m_Dots.push_back(CPoint(GetRandomNumber(0, m_nImageWidth), GetRandomNumber(0, m_nImageHeight)));
	MakeImageDots(pszImage);
}

//화면 내 위치 검증
bool CiglimDlg::ValidImagePos(CPoint point)
{
	bool bRet = false;

	if( point.x >= 0 && point.x < m_nImageWidth && point.y >= 0 && point.y < m_nImageHeight)
		bRet = true;

	return bRet;
}

//세팅 변경시 그림 다시 그리기
void CiglimDlg::MakeImageCurSettings(unsigned char* pszImage)
{
	SetClearImage();
	MakeImageDots(pszImage);
	MakeImageCircleOutlineWithDots(pszImage);
}

//화면 지우기
void CiglimDlg::ClearScreen()
{
	Invalidate();
	UpdateWindow();
}

//깨끗한 이미지 생성
void CiglimDlg::SetClearImage()
{
	if (m_nBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_Image.SetColorTable(0, 256, rgb);
	}
	
	memset(m_Image.GetBits(), 0xff, m_nImageWidth * m_nImageHeight);
}

//화면 그리기
void CiglimDlg::DrawScreen(unsigned char* pszImage)
{
	CClientDC dc(this);

	if (pszImage == nullptr)
		pszImage = (unsigned char*)m_Image.GetBits();
	
	m_Image.Draw(dc, 0, 0, m_nImageWidth, m_nImageHeight);
}

//클릭 지점 확인
bool CiglimDlg::IsItDot(CPoint point)
{
	bool bRet = false;

	for (CPoint Dot : m_Dots) {
		if (IsInCircle(point, Dot, m_nDotRadSize)) {
			m_Target = Dot;
			bRet = true;
			break;
		}
	}

	return bRet;
}

//점, 정원 반복 생성
DWORD CiglimDlg::RepeatGenerateCircles() {
	CClientDC dc(this);
	unsigned char* pszImage = (unsigned char*)m_Image.GetBits();

	for (int i = 0; i < 10; i++) {
		RemoveDots();
		SetClearImage();

		GenerateImageRandomDots(pszImage);
		MakeImageCircleOutlineWithDots(pszImage);
		DrawScreen(pszImage);

		Sleep(2000);
	}

	return 0;
}

/**
 * 버튼
 * 버튼 이벤트 함수 구현부 입니다.
 */

 //클릭시 점 설정
void CiglimDlg::OnBnClickedBtnSetting()
{
	CClientDC dc(this);
	unsigned char* pszImage = (unsigned char*)m_Image.GetBits();
	CSettingDlg SettingDlg;
	CString strData = _T("");
	strData.Format(_T("%d"), m_nDotRadSize);
	SettingDlg.m_strDotRadSize = strData;
	strData.Format(_T("%d"), m_nThickness);
	SettingDlg.m_strThickness = strData;


	if (SettingDlg.DoModal() == IDOK) {
		m_nDotRadSize = _ttoi(SettingDlg.m_strDotRadSize);
		m_nThickness = _ttoi(SettingDlg.m_strThickness);

		//점 변경 완료되면 기존 점 크기 변경
		MakeImageCurSettings(pszImage);
		DrawScreen(pszImage);
	}
}

//리셋
void CiglimDlg::OnBnClickedBtnReset()
{
	CClientDC dc(this);

	RemoveDots();
	SetClearImage();

	DrawScreen((unsigned char*)m_Image.GetBits());
}

//랜덤
void CiglimDlg::OnBnClickedBtnRandom()
{
	CClientDC dc(this);
	unsigned char* pszImage = (unsigned char*)m_Image.GetBits();

	RemoveDots();
	SetClearImage();

	GenerateImageRandomDots(pszImage);
	MakeImageCircleOutlineWithDots(pszImage);

	DrawScreen(pszImage);
}

//반복
void CiglimDlg::OnBnClickedBtnRepeat()
{
	thread RepeatThread;
	
	GetDlgItem(IDC_BTN_REPEAT)->EnableWindow(FALSE);
	
	RepeatThread = std::thread([this]() {
		RepeatGenerateCircles();
		m_bThreadStatus = false;
		PostMessage(WM_THREAD_ACTIVE); // 사용자 정의 메시지로 버튼 복구
		});

	RepeatThread.detach();
}

//마우스 Down 이벤트
void CiglimDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	unsigned char* pszImage = (unsigned char*)m_Image.GetBits();
	m_bLeftClick = true;
	//현재 화면이 찍힌 점이 3개 미만일때만
	if (m_Dots.size() < 3) {
		MakeImageDot(pszImage, point);
		MakeImageCircleOutlineWithDots(pszImage);

		DrawScreen(pszImage);
	}

	if (IsItDot(point)) {
		m_bDot = true;
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

//마우스 Up 이벤트
void CiglimDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLeftClick = false;
	m_bDot = false;
	m_Target = (-1, -1);

	CDialogEx::OnLButtonUp(nFlags, point);
}

//마우스 Move 이벤트
void CiglimDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	unsigned char* pszImage = (unsigned char*)m_Image.GetBits();
	vector<CPoint>::iterator iter;

	if (m_Dots.size() == 3 && m_bLeftClick && m_bDot && ValidImagePos(point)) {
		SetClearImage();

		iter = find(m_Dots.begin(), m_Dots.end(), m_Target);
		if (iter != m_Dots.end() && m_Target != point) {
			m_Dots.erase(iter);
			MakeImageDot(pszImage, point);
			m_Target = point;
		}
		MakeImageDots(pszImage);
		MakeImageCircleOutlineWithDots(pszImage);

		DrawScreen(pszImage);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

//Thread Acvtive 메시지
LRESULT CiglimDlg::OnThreadActive(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(IDC_BTN_REPEAT)->EnableWindow(TRUE);
	return 0;
}