#pragma once
#include "Utils.h"

#define RADIUS	10

struct ColorInfo
{
	int m_red;
	int m_green;
	int m_blue;
};

struct ShapeInfo
{
	CFPoint m_center;
	CFPoint m_size;
	int m_nThick;
	RGBQUAD m_color[256];
};

class GShape
{
public:
	GShape(CFPoint center);
	virtual ~GShape();

	virtual void Draw(CDC* pDC) = 0;
	virtual bool IsInShape(const CFPoint& currentPos, const CFPoint& center) { return false; };

	void SetCenter(CFPoint center) { m_shapeInfo.m_center = center; }
	CFPoint GetCenter() { return m_shapeInfo.m_center; }
	void SetSize(CFPoint size) { m_shapeInfo.m_size = size; }
	CFPoint GetSize() { return m_shapeInfo.m_size; }

protected:
	CImage m_image;
	ShapeInfo m_shapeInfo;
	int m_nBPP;
};

class GRectangle : public GShape
{
public:
	GRectangle(CFPoint center, CFPoint size);
	~GRectangle();

	virtual void Draw(CDC* pDC) override;
};

class GCircle : public GShape
{
public:
	GCircle(CFPoint center, float nRadius, int nThick = 0, bool bText = true);
	~GCircle();

	virtual void Draw(CDC* pDC) override;
	void DrawText(CDC* pDC);
	bool IsInShape(const CFPoint& currentPos, const CFPoint& center) override;

private:
	bool m_bText;

};

