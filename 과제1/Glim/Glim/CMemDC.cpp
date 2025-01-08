#include "pch.h"
#include "CMemDC.h"

CGlimMemDC::CGlimMemDC(CDC* pDC)
{
	m_pDC = pDC;

}

CGlimMemDC::~CGlimMemDC()
{
	
}

void CGlimMemDC::Draw(const RECT& rcDrawClient, const CRect& rcWndClient)
{
	CDC memDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap;
    CPen pen(PS_SOLID, 2, RGB(255, 0, 0)); 
    CBrush brush(RGB(0, 255, 0));          
    CPen* pOldPen;
    CBrush* pOldBrush;


	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC, rcWndClient.Width(), rcWndClient.Height());
	pOldBitmap = memDC.SelectObject(&bitmap);

	memDC.BitBlt(0, 0, rcWndClient.Width(), rcWndClient.Height(), m_pDC, 0, 0, SRCCOPY);

    pOldPen = memDC.SelectObject(&pen);
    pOldBrush = memDC.SelectObject(&brush);

    memDC.Rectangle(&rcDrawClient);

    memDC.SelectObject(pOldPen);
    memDC.SelectObject(pOldBrush);

    m_pDC->BitBlt(0, 0, rcWndClient.Width(), rcWndClient.Height(), &memDC, 0, 0, SRCCOPY);

    memDC.SelectObject(pOldBitmap);

}

CGlimEllipseMemDC::CGlimEllipseMemDC(CDC* pDC) : CGlimMemDC(pDC)
{

}

CGlimEllipseMemDC::~CGlimEllipseMemDC()
{
}

void CGlimEllipseMemDC::Draw(const RECT& rcDrawClient, const CRect& rcWndClient)
{
    CDC memDC;
    CBitmap bitmap;
    CBitmap* pOldBitmap;
    CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
    CBrush brush(RGB(0, 255, 0));
    CPen* pOldPen;
    CBrush* pOldBrush;


    memDC.CreateCompatibleDC(m_pDC);
    bitmap.CreateCompatibleBitmap(m_pDC, rcWndClient.Width(), rcWndClient.Height());
    pOldBitmap = memDC.SelectObject(&bitmap);

    memDC.BitBlt(0, 0, rcWndClient.Width(), rcWndClient.Height(), m_pDC, 0, 0, SRCCOPY);

    pOldPen = memDC.SelectObject(&pen);
    pOldBrush = memDC.SelectObject(&brush);

    memDC.Ellipse(&rcDrawClient);

    memDC.SelectObject(pOldPen);
    memDC.SelectObject(pOldBrush);

    m_pDC->BitBlt(rcWndClient.left, rcWndClient.top, rcWndClient.Width(), rcWndClient.Height(), &memDC, 0, 0, SRCCOPY);

    memDC.SelectObject(pOldBitmap);
}
