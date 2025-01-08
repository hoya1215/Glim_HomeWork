
#pragma once
#include <vector>
#include <thread>

#define MAX_SHAPE 3

class GRectangle;
class GShape;
class GCircle;

enum class ESTATE
{
	BIG_CIRCLE,
	NONE
};

class CGlimDlg : public CDialogEx
{
public:
	CGlimDlg(CWnd* pParent = nullptr);	
	virtual ~CGlimDlg();

	void DrawClickCircle(CPoint ptPoint);
	void DrawBigCircle(CDC* pDC);
	void DrawAllCircle(CDC* pDC);
	void RandomMoveClickCircle();

	int IsMouseInShape(CPoint ptPoint);
	void Reset();


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLIM_DIALOG };
#endif


protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedRandomMove();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();

private:

	GCircle* m_pCircle;
	std::vector<GShape*> m_vShape;

	CPoint m_ptStartCenter;
	CPoint m_ptEndCenter;
	int m_nCount;
	int m_nRadius;
	
	ESTATE m_state;

	CPoint m_ptPrevMousePos;

	COLORREF m_clrDefault;
	static int m_nIndex;
};
