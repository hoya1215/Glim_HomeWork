
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

int CGlimDlg::m_nIndex = -1;

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
	m_pCircle = nullptr;
	m_state = ESTATE::NONE;
	m_clrDefault = GetSysColor(COLOR_3DFACE);

	//_CrtSetBreakAlloc(2300);
}

CGlimDlg::~CGlimDlg()
{
	for (int i = 0; i < m_vShape.size(); ++i)
		delete m_vShape[i];

	m_vShape.clear();

	delete m_pCircle;
	m_pCircle = nullptr;
}



void CGlimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGlimDlg, CDialogEx)
	ON_WM_PAINT()
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

void CGlimDlg::DrawClickCircle(CPoint ptPoint)
{
	int nRadius = 10;
	CFPoint ptCenter = { ptPoint.x , ptPoint.y };

	GCircle* pCircle = new GCircle(ptCenter, nRadius);
	m_vShape.push_back(pCircle);
	Invalidate();
}

void CGlimDlg::DrawBigCircle(CDC* pDC)
{
	if (m_pCircle)
	{
		delete m_pCircle;
		m_pCircle = nullptr;
	}

	CFPoint ptBigCenter;
	float fRadius;
	CFPoint ptSmallCenter1 = m_vShape[0]->GetCenter();
	CFPoint ptSmallCenter2 = m_vShape[1]->GetCenter();
	CFPoint ptSmallCenter3 = m_vShape[2]->GetCenter();

	// 안되는 조건 1
	if (ptSmallCenter1 == ptSmallCenter2 || ptSmallCenter2 == ptSmallCenter3 || ptSmallCenter1 == ptSmallCenter3)
		return;

	// 중점
	CFPoint ptCenter1 = { (ptSmallCenter1.x + ptSmallCenter2.x) / 2, (ptSmallCenter1.y + ptSmallCenter2.y) / 2 };
	CFPoint ptCenter2 = { (ptSmallCenter2.x + ptSmallCenter3.x) / 2, (ptSmallCenter2.y + ptSmallCenter3.y) / 2 };

	// 기울기
	float fSlope1, fSlope2;
	if (ptSmallCenter2.x - ptSmallCenter1.x != 0)
		fSlope1 = (float)(ptSmallCenter2.y - ptSmallCenter1.y) / (float)(ptSmallCenter2.x - ptSmallCenter1.x);
	else
		fSlope1 = (float)(ptSmallCenter2.y - ptSmallCenter1.y);

	if (ptSmallCenter3.x - ptSmallCenter2.x != 0)
		fSlope2 = (float)(ptSmallCenter3.y - ptSmallCenter2.y) / (float)(ptSmallCenter3.x - ptSmallCenter2.x);
	else
		fSlope2 = (float)(ptSmallCenter3.y - ptSmallCenter2.y);

	// 안되는 조건 2
	if (fSlope1 == fSlope2)
		return;

	// 수직 이등분 선의 기울기
	float fPerp1, fPerp2;

	if (fSlope1 != 0)
		fPerp1 = -1 / fSlope1;
	else
		fPerp1 = (float)(ptSmallCenter2.x + ptSmallCenter1.x) / 2;

	if (fSlope2 != 0)
		fPerp2 = -1 / fSlope2;
	else
		fPerp2 = (float)(ptSmallCenter2.x + ptSmallCenter3.x) / 2;


	ptBigCenter.x = (ptCenter1.y - ptCenter2.y - fPerp1 * ptCenter1.x + fPerp2 * ptCenter2.x) / (fPerp2 - fPerp1);
	ptBigCenter.y = fPerp1 * (ptBigCenter.x - ptCenter1.x) + ptCenter1.y;

	fRadius = sqrt((ptSmallCenter1.x - ptBigCenter.x) * (ptSmallCenter1.x - ptBigCenter.x) +
		(ptSmallCenter1.y - ptBigCenter.y) * (ptSmallCenter1.y - ptBigCenter.y));

	if (!m_pCircle)
		m_pCircle = new GCircle(ptBigCenter, fRadius, CIRCLE_THICK, false);

	m_pCircle->Draw(pDC);
}

void CGlimDlg::DrawAllCircle(CDC* pDC)
{
	CDC memDC;
	CRect clientRect;
	CBitmap bitmap;
	CBitmap* pOldBitmap;

	GetClientRect(&clientRect);

	// 더블 버퍼링
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
		int nX = xPos(gen);
		int nY = yPos(gen);
		CFPoint ptCenter = { nX, nY };
		m_vShape[i]->SetCenter(ptCenter);
	}

	Invalidate(FALSE);
}

int CGlimDlg::IsMouseInShape(CPoint ptPoint)
{
	CFPoint ptCurrentPos = { ptPoint.x, ptPoint.y };

	for (int i = 0; i < m_vShape.size(); ++i)
	{
		if (m_vShape[i]->IsInShape(ptCurrentPos, m_vShape[i]->GetCenter()))
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

	m_nIndex = -1;
}




BOOL CGlimDlg::PreTranslateMessage(MSG* pMsg)
{
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CGlimDlg::OnOK()
{
	//CDialogEx::OnOK();
}


void CGlimDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_vShape.size() >= MAX_SHAPE)
		m_state = ESTATE::BIG_CIRCLE;

	switch (m_state)
	{
	case ESTATE::BIG_CIRCLE:
		m_nIndex = IsMouseInShape(point);
		m_ptPrevMousePos = point;
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
	}

	m_vShape.clear();
	delete m_pCircle;
	m_pCircle = nullptr;
	Reset();
	Invalidate(FALSE);
}

void CGlimDlg::OnBnClickedRandomMove()
{

	std::thread _thread1(ThreadRandomMove, this);
	//std::thread _thread2(ThreadRandomMove, this);

	_thread1.detach();
	//_thread2.detach();
}


void CGlimDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_nIndex == -1)
		return;

	CFPoint ptMouseDist = { point.x - m_ptPrevMousePos.x , point.y - m_ptPrevMousePos.y };


	CFPoint ptShapeCenter = m_vShape[m_nIndex]->GetCenter();
	CFPoint ptShapeCenter2 = { ptShapeCenter.x + ptMouseDist.x, ptShapeCenter.y + ptMouseDist.y };
	m_vShape[m_nIndex]->SetCenter(ptShapeCenter2);
	Invalidate(FALSE);

	m_ptPrevMousePos = point;

	CDialogEx::OnMouseMove(nFlags, point);
}


void CGlimDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	Reset();

	CDialogEx::OnLButtonUp(nFlags, point);
}
