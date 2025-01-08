
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
// 생성입니다.
public:
	CGlimDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	virtual ~CGlimDlg();

	void DrawClickCircle(CPoint point);
	void DrawBigCircle(CDC* pDC);
	void DrawAllCircle(CDC* pDC);
	void RandomMoveClickCircle();

	int IsMouseInShape(CPoint point);
	void Reset();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLIM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	//afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedRandomMove();

	DECLARE_MESSAGE_MAP()


private:

	GCircle* m_circle;
	std::vector<GShape*> m_vShape;

	CPoint m_startCenter;
	CPoint m_endCenter;
	int m_nCount;
	int m_nRadius;
	
	ESTATE m_state;

	CPoint m_prevMousePos;

	COLORREF m_clrDefault;
	static int nIndex;

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
