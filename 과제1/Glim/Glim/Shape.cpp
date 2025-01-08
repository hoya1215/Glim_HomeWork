#include "pch.h"
#include "Shape.h"

#define TEXT_GAP 15

GShape::GShape(CFPoint center)
{
	m_shapeInfo.m_center = center;
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


GRectangle::GRectangle(CFPoint center, CFPoint size)
	: GShape(center)
{
	m_shapeInfo.m_size = size;
	m_image.Create(m_shapeInfo.m_size.x, -m_shapeInfo.m_size.y, m_nBPP);
}

GRectangle::~GRectangle()
{
}

void GRectangle::Draw(CDC* pDC)
{
	m_image.SetColorTable(0, 256, m_shapeInfo.m_color);
	
	m_image.Draw(*pDC, 0, 0);
}

GCircle::GCircle(CFPoint center, float nRadius, int nThick, bool bText)
	: GShape(center)
{
	m_shapeInfo.m_size = { nRadius * 2, nRadius * 2 };
	if (nThick > nRadius)
		nThick = nRadius;

	m_shapeInfo.m_nThick = nThick;
	m_bText = bText;

	m_image.Create(m_shapeInfo.m_size.x, -m_shapeInfo.m_size.y, m_nBPP);
}

GCircle::~GCircle()
{
}

void GCircle::Draw(CDC* pDC)
{
	m_image.SetHasAlphaChannel(1);
	//m_image.SetColorTable(0, 256, m_shapeInfo.m_color);
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();

	unsigned char* pixel = (unsigned char*)m_image.GetBits();
	memset(pixel, 0xff, nWidth * nHeight);

	for(int j = 0; j < nHeight; ++j)
		for (int i = 0; i < nWidth; ++i)
		{
			CFPoint currentPos = { i, j };
			CFPoint center = { m_shapeInfo.m_size.x / 2, m_shapeInfo.m_size.y / 2 };

			if (IsInShape(currentPos, center))
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

	
	m_image.Draw(*pDC, m_shapeInfo.m_center.x - m_shapeInfo.m_size.x / 2, m_shapeInfo.m_center.y - m_shapeInfo.m_size.y / 2);
	
	if(m_bText)
		DrawText(pDC);
}

void GCircle::DrawText(CDC* pDC)
{

	pDC->SetBkMode(TRANSPARENT);

	CString textCenter;
	textCenter.Format(_T("x : %.0f    y : %.0f"), m_shapeInfo.m_center.x, m_shapeInfo.m_center.y);
	CRect rect(m_shapeInfo.m_center.x - 100, m_shapeInfo.m_center.y - m_shapeInfo.m_size.y - TEXT_GAP,
		m_shapeInfo.m_center.x + 100, m_shapeInfo.m_center.y - TEXT_GAP);

	pDC->DrawText(textCenter, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

bool GCircle::IsInShape(const CFPoint& currentPos, const CFPoint& center)
{
	float dX = center.x - currentPos.x;
	float dY = center.y - currentPos.y;
	float dist = dX * dX + dY * dY;
	float radius = m_shapeInfo.m_size.x / 2;

	if (m_shapeInfo.m_nThick != 0 && dist < (radius - m_shapeInfo.m_nThick) * (radius - m_shapeInfo.m_nThick))
		return false;

	if (dist < radius*radius)
		return true;

	return false;
}
