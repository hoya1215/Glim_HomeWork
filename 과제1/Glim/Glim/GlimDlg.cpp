
// GlimDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Glim.h"
#include "GlimDlg.h"
#include "afxdialogex.h"
#include "CMemDC.h"
#include "Shape.h"
#include <random>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CIRCLE_THICK 4

int CGlimDlg::nIndex = -1;

void ThreadRandomMove(CGlimDlg* pGlimDlg)
{
	for (int i = 0; i < 10; ++i)
	{
		pGlimDlg->RandomMoveClickCircle();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

CGlimDlg::CGlimDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GLIM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nRadius = 10;
	m_nCount = 30;
	m_circle = nullptr;
	m_state = ESTATE::NONE;
	m_clrDefault = GetSysColor(COLOR_3DFACE);

	//_CrtSetBreakAlloc(2300);
}

CGlimDlg::~CGlimDlg()
{
	for (int i = 0; i < m_vShape.size(); ++i)
		delete m_vShape[i];

	m_vShape.clear();

	delete m_circle;
	m_circle = nullptr;
}

void CGlimDlg::DrawClickCircle(CPoint point)
{
	int radius = 10;
	CFPoint center = { point.x , point.y };

	GCircle* circle = new GCircle(center, radius);
	m_vShape.push_back(circle);
	Invalidate();
}

void CGlimDlg::DrawBigCircle(CDC* pDC)
{
	if (m_circle)
	{
		delete m_circle;
		m_circle = nullptr;
	}

	CFPoint bigCenter;
	float radius;
	CFPoint smallCenter1 = m_vShape[0]->GetCenter();
	CFPoint smallCenter2 = m_vShape[1]->GetCenter();
	CFPoint smallCenter3 = m_vShape[2]->GetCenter();

	// 안되는 조건 1
	if (smallCenter1 == smallCenter2 || smallCenter2 == smallCenter3 || smallCenter1 == smallCenter3)
		return;

	// 두 변의 중점
	CFPoint center1 = { (smallCenter1.x + smallCenter2.x) / 2, (smallCenter1.y + smallCenter2.y) / 2 };
	CFPoint center2 = { (smallCenter2.x + smallCenter3.x) / 2, (smallCenter2.y + smallCenter3.y) / 2 };

	// 두 변의 기울기
	float slope1, slope2;
	if (smallCenter2.x - smallCenter1.x != 0)
		slope1 = (float)(smallCenter2.y - smallCenter1.y) / (float)(smallCenter2.x - smallCenter1.x);
	else
		slope1 = (float)(smallCenter2.y - smallCenter1.y);

	if (smallCenter3.x - smallCenter2.x != 0)
		slope2 = (float)(smallCenter3.y - smallCenter2.y) / (float)(smallCenter3.x - smallCenter2.x);
	else
		slope2 = (float)(smallCenter3.y - smallCenter2.y);

	// 안되는 조건 2
	if (slope1 == slope2)
		return;

	// 수직 이등분 선의 기울기
	float perp1, perp2;

	if (slope1 != 0)
		perp1 = -1 / slope1;
	else
		perp1 = (float)(smallCenter2.x + smallCenter1.x) / 2;

	if (slope2 != 0)
		perp2 = -1 / slope2;
	else
		perp2 = (float)(smallCenter2.x + smallCenter3.x) / 2;



	bigCenter.x = (center1.y - center2.y - perp1 * center1.x + perp2 * center2.x) / (perp2 - perp1);
	bigCenter.y = perp1 * (bigCenter.x - center1.x) + center1.y;

	radius = sqrt((smallCenter1.x - bigCenter.x) * (smallCenter1.x - bigCenter.x) +
		(smallCenter1.y - bigCenter.y) * (smallCenter1.y - bigCenter.y));

	if(!m_circle)
		m_circle = new GCircle(bigCenter, radius, CIRCLE_THICK, false);

	m_circle->Draw(pDC);
}

void CGlimDlg::DrawAllCircle(CDC* pDC)
{
	CDC memDC;
	CRect clientRect;
	CBitmap bitmap;
	CBitmap* pOldBitmap;

	GetClientRect(&clientRect);


	memDC.CreateCompatibleDC(pDC);
	bitmap.CreateCompatibleBitmap(pDC, clientRect.Width(), clientRect.Height());
	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(clientRect, m_clrDefault);

	for (int i = 0; i < m_vShape.size(); ++i)
		m_vShape[i]->Draw(&memDC);

	if (m_vShape.size() >= MAX_SHAPE)
		DrawBigCircle(&memDC);

	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
}

void CGlimDlg::RandomMoveClickCircle()
{
	std::random_device rd;  
	std::mt19937 gen(rd()); 
	CRect clientRect;
	GetClientRect(&clientRect);

	std::uniform_int_distribution<int> xPos(0, clientRect.Width());
	std::uniform_int_distribution<int> yPos(0, clientRect.Height());



	for (int i = 0; i < m_vShape.size(); ++i)
	{
		int x = xPos(gen);
		int y = yPos(gen);
		CFPoint center = { x, y };
		m_vShape[i]->SetCenter(center);
	}

	Invalidate(FALSE);
}

int CGlimDlg::IsMouseInShape(CPoint point)
{
	CFPoint currentPos = { point.x, point.y };

	for (int i = 0; i < m_vShape.size(); ++i)
	{

		if (m_vShape[i]->IsInShape(currentPos, m_vShape[i]->GetCenter()))
			return i;
	}

	return -1;
}

void CGlimDlg::Reset()
{
	if (m_vShape.size() >= MAX_SHAPE)
		m_state = ESTATE::BIG_CIRCLE;
	else
		m_state = ESTATE::NONE;

	nIndex = -1;
}

void CGlimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGlimDlg, CDialogEx)
	//ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CGlimDlg::OnBnClickedReset)
	ON_BN_CLICKED(IDC_BUTTON_RANDOM_MOVE, &CGlimDlg::OnBnClickedRandomMove)
END_MESSAGE_MAP()



BOOL CGlimDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	return TRUE; 
}


void CGlimDlg::OnPaint()
{
	// 더블 버퍼링
	//CPaintDC dc(this);
	//RECT rect;
	//RECT rcClient;
	//RECT rcRange;

	//rect = { m_startCenter.x - m_nRadius, m_startCenter.y - m_nRadius, m_startCenter.x + m_nRadius, m_startCenter.y + m_nRadius };
	//GetClientRect(&rcClient);
	//rcRange = { rcClient.left, rcClient.top + 100, rcClient.right, rcClient.bottom };

	//CGlimEllipseMemDC memDC(&dc);
	//memDC.Draw(rect, rcRange);


	CPaintDC dc(this);
	DrawAllCircle(&dc);

	CDialogEx::OnPaint();
}

HCURSOR CGlimDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CGlimDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CGlimDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	//CDialogEx::OnOK();
}


void CGlimDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_vShape.size() >= MAX_SHAPE)
		m_state = ESTATE::BIG_CIRCLE;

	switch (m_state)
	{
	case ESTATE::BIG_CIRCLE:
		nIndex = IsMouseInShape(point);
		m_prevMousePos = point;
		break;
	case ESTATE::NONE:
		DrawClickCircle(point);
	}


	CDialogEx::OnLButtonDown(nFlags, point);
}

void CGlimDlg::OnBnClickedReset()
{
	for (int i = 0; i < m_vShape.size(); ++i)
	{
		delete m_vShape[i];
		m_vShape[i] = nullptr;
	}

	m_vShape.clear();
	delete m_circle;
	m_circle = nullptr;
	Reset();
	Invalidate(FALSE);
}

void CGlimDlg::OnBnClickedRandomMove()
{

	std::thread _thread1(ThreadRandomMove, this);
	//std::thread _thread2(ThreadRandomMove, this);

	_thread1.detach();
	//_thread2.detach();






	//RandomMoveClickCircle();
}


void CGlimDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nIndex == -1)
		return;

	CFPoint mouseDist = { point.x - m_prevMousePos.x , point.y - m_prevMousePos.y };


	CFPoint shapeCenter = m_vShape[nIndex]->GetCenter();
	CFPoint shapeCenter2 = { shapeCenter.x + mouseDist.x, shapeCenter.y + mouseDist.y };
	m_vShape[nIndex]->SetCenter(shapeCenter2);
	Invalidate(FALSE);

	m_prevMousePos = point;

	CDialogEx::OnMouseMove(nFlags, point);
}


void CGlimDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	Reset();

	CDialogEx::OnLButtonUp(nFlags, point);
}
