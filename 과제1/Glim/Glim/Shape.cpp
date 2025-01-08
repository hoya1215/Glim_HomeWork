#include "pch.h"
#include "Shape.h"

#define TEXT_GAP 15

GShape::GShape(CFPoint ptCenter)
{
	m_shapeInfo.m_ptCenter = ptCenter;
	m_nBPP = 32;

	for (int i = 0; i < 256; ++i)
	{
		m_shapeInfo.m_color[i].rgbRed = i;
		m_shapeInfo.m_color[i].rgbGreen = i;
		m_shapeInfo.m_color[i].rgbBlue = i;
	}
}

GShape::~GShape()
{
}


GRectangle::GRectangle(CFPoint ptCenter, CFPoint ptSize)
	: GShape(ptCenter)
{
	m_shapeInfo.m_ptSize = ptSize;
	m_image.Create(m_shapeInfo.m_ptSize.x, -m_shapeInfo.m_ptSize.y, m_nBPP);
}

GRectangle::~GRectangle()
{
}

void GRectangle::Draw(CDC* pDC)
{
	m_image.SetColorTable(0, 256, m_shapeInfo.m_color);
	m_image.Draw(*pDC, 0, 0);
}

GCircle::GCircle(CFPoint ptCenter, float fRadius, int nThick, bool bText)
	: GShape(ptCenter)
{
	m_shapeInfo.m_ptSize = { fRadius * 2, fRadius * 2 };
	if (nThick > fRadius)
		nThick = fRadius;

	m_shapeInfo.m_nThick = nThick;
	m_bText = bText;

	m_image.Create(m_shapeInfo.m_ptSize.x, -m_shapeInfo.m_ptSize.y, m_nBPP);
}

GCircle::~GCircle()
{
}

void GCircle::Draw(CDC* pDC)
{
	m_image.SetHasAlphaChannel(1);

	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();

	unsigned char* pixel = (unsigned char*)m_image.GetBits();
	memset(pixel, 0xff, nWidth * nHeight);

	for(int j = 0; j < nHeight; ++j)
		for (int i = 0; i < nWidth; ++i)
		{
			CFPoint ptCurrentPos = { i, j };
			CFPoint ptCenter = { m_shapeInfo.m_ptSize.x / 2, m_shapeInfo.m_ptSize.y / 2 };

			if (IsInShape(ptCurrentPos, ptCenter))
			{

				pixel[j * nPitch + i * 4] = 0;
				pixel[j * nPitch + i * 4 + 1] = 0;
				pixel[j * nPitch + i * 4 + 2] = 0;
				pixel[j * nPitch + i * 4 + 3] = 255;
			}
			else
			{
				pixel[j * nPitch + i * 4] = 0;
				pixel[j * nPitch + i * 4 + 1] = 0;
				pixel[j * nPitch + i * 4 + 2] = 0;
				pixel[j * nPitch + i * 4 + 3] = 0;
			}
		}

	
	m_image.Draw(*pDC, m_shapeInfo.m_ptCenter.x - m_shapeInfo.m_ptSize.x / 2, m_shapeInfo.m_ptCenter.y - m_shapeInfo.m_ptSize.y / 2);
	
	if(m_bText)
		DrawText(pDC);
}

void GCircle::DrawText(CDC* pDC)
{

	pDC->SetBkMode(TRANSPARENT);

	CString textCenter;
	textCenter.Format(_T("x : %.0f    y : %.0f"), m_shapeInfo.m_ptCenter.x, m_shapeInfo.m_ptCenter.y);
	CRect rect(m_shapeInfo.m_ptCenter.x - 100, m_shapeInfo.m_ptCenter.y - m_shapeInfo.m_ptSize.y - TEXT_GAP,
		m_shapeInfo.m_ptCenter.x + 100, m_shapeInfo.m_ptCenter.y - TEXT_GAP);

	pDC->DrawText(textCenter, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

bool GCircle::IsInShape(const CFPoint& ptCurrentPos, const CFPoint& ptCenter)
{
	float fX = ptCenter.x - ptCurrentPos.x;
	float fY = ptCenter.y - ptCurrentPos.y;
	float fDist = fX * fX + fY * fY;
	float fRadius = m_shapeInfo.m_ptSize.x / 2;

	if (m_shapeInfo.m_nThick != 0 && fDist < (fRadius - m_shapeInfo.m_nThick) * (fRadius - m_shapeInfo.m_nThick))
		return false;

	if (fDist < fRadius*fRadius)
		return true;

	return false;
}
