
// iglimDlg.h: 헤더 파일
//
#include "CSettingDlg.h"
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <thread>

#pragma once
#define WM_THREAD_ACTIVE (WM_USER + 100)	//쓰레드 활성화 메시지

// CiglimDlg 대화 상자
class CiglimDlg : public CDialogEx
{
// 생성입니다.
public:
	CiglimDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IGLIM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
public:
	int m_nDotRadSize;				//점 크기
	int m_nThickness;				//원 두께
	std::vector<CPoint> m_Dots;		//원 그리기에 사용할 점(최대 3개)

private:
	//이미지 기본 설정 변수
	int m_nBpp = 8;
	CImage m_Image;
	int m_nImageWidth;		//이미지 가로 크기
	int m_nImageHeight;		//이미지 세로 크기

	CRect m_rect;

	//드래그 기능 변수
	bool m_bLeftClick;
	bool m_bDot;
	CPoint m_Target;

	bool m_bThreadStatus;			//쓰레드 가용 상태

private:
	//원에 관련된 함수
	bool IsInCircle(CPoint Dot, CPoint Center, int nRadius);									//현재 픽셀 위치 확인
	CPoint CalcCircleCenter(CPoint DotA, CPoint DotB, CPoint DotC);								//3개 좌표로 그릴 원 중앙 계산
	bool IsOnLine(CPoint Dot, CPoint Center, int nRadius);										//원의 선 위에 있는지 확인
	
	//도형 이미지 생성 함수
	void MakeImageDot(unsigned char* psz, CPoint Point);												//점 이미지 생성
	void MakeImageDots(unsigned char* psz);																//저장된 점 이미지 생성
	void MakeImageCircle(unsigned char* psz, CPoint Center, int nRadius = 1, int nGray = 0);			//원 이미지 생성
	void MakeImageCircleOutline(unsigned char* psz, CPoint Center, int nRadius = 1, int nGray = 0);		//외곽선 있는 원 이미지 생성
	void MakeImageCircleOutlineWithDots(unsigned char* psz);											//저장된 점으로 외곽선 있는 원 이미지 생성
	void MakeImageCurSettings(unsigned char* psz);														//현재 설정대로 이미지 생성

	//점에 관련된 함수
	void RemoveDots();										//점 초기화
	int	GetRandomNumber(int min, int max);					//범위 난수 생성
	void GenerateImageRandomDots(unsigned char* psz);		//랜덤 점 생성
	
	//기본적인 그리기에 필요한 함수
	bool ValidImagePos(CPoint point);			//화면 내 위치 검증

	//화면 갱신시 사용 함수
	void ClearScreen();							//기본 화면 설정
	void SetClearImage();						//깨끗한 이미지 생성
	void DrawScreen(unsigned char* pszImage);	//화면 그리기

	//드래그 기능에 필요한 함수
	bool IsItDot(CPoint point);					//클릭 지점 확인

	//쓰레드
	DWORD RepeatGenerateCircles();				//점, 정원 반복 생성

protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	//버튼
	afx_msg void OnBnClickedBtnSetting();							//클릭시 점 설정
	afx_msg void OnBnClickedBtnReset();								//리셋
	afx_msg void OnBnClickedBtnRandom();							//랜덤
	afx_msg void OnBnClickedBtnRepeat();							//반복
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);			//마우스 Down 이벤트
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);			//마우스 Up 이벤트
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);			//마우스 Move 이벤트
	afx_msg LRESULT OnThreadActive(WPARAM wParam, LPARAM lParam);	//Thread Acvtive 메시지
	
};
