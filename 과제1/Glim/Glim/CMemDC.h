#pragma once

class CGlimMemDC
{
public:
	CGlimMemDC(CDC* pDC);
	virtual ~CGlimMemDC();

	virtual void Draw(const RECT& rcDrawClient, const CRect& rcWndClient);

protected:
	CDC* m_pDC;

};

class CGlimEllipseMemDC : public CGlimMemDC 
{
public:
	CGlimEllipseMemDC(CDC* pDC);
	~CGlimEllipseMemDC();

	virtual void Draw(const RECT& rcDrawClient, const CRect& rcWndClient) override;

};

