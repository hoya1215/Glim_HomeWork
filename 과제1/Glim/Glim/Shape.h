#pragma once
#include "Utils.h"

#define RADIUS	10

struct ColorInfo
{
	int m_nRed;
	int m_nGreen;
	int m_nBlue;
};

struct ShapeInfo
{
	CFPoint m_ptCenter;
	CFPoint m_ptSize;
	int m_nThick;
	RGBQUAD m_color[256];
};

class GShape
{
public:
	GShape(CFPoint ptCenter);
	virtual ~GShape();

	virtual void Draw(CDC* pDC) = 0;
	virtual bool IsInShape(const CFPoint& ptCurrentPos, const CFPoint& ptCenter) { return false; };

	void SetCenter(CFPoint ptCenter) { m_shapeInfo.m_ptCenter = ptCenter; }
	CFPoint GetCenter() { return m_shapeInfo.m_ptCenter; }
	void SetSize(CFPoint ptSize) { m_shapeInfo.m_ptSize = ptSize; }
	CFPoint GetSize() { return m_shapeInfo.m_ptSize; }

protected:
	CImage m_image;
	ShapeInfo m_shapeInfo;
	int m_nBPP;
};

class GRectangle : public GShape
{
public:
	GRectangle(CFPoint ptCenter, CFPoint ptSize);
	~GRectangle();

	virtual void Draw(CDC* pDC) override;
};

class GCircle : public GShape
{
public:
	GCircle(CFPoint ptCenter, float fRadius, int nThick = 0, bool bText = true);
	~GCircle();

	virtual void Draw(CDC* pDC) override;
	void DrawText(CDC* pDC);
	bool IsInShape(const CFPoint& ptCurrentPos, const CFPoint& ptCenter) override;

private:
	bool m_bText;

};

