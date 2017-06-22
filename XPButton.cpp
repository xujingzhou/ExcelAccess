// XPButton.cpp : implementation file
// Modified by jingzhou xu

#include "stdafx.h"
#include "XPButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXPButton

IMPLEMENT_DYNCREATE(CXPButton, CButton)


CXPButton::CXPButton()
{
	pBoundryPen.CreatePen(PS_INSIDEFRAME|PS_SOLID,1,RGB(0,0,0));
	pInsideBoundryPenLeft.CreatePen(PS_INSIDEFRAME|PS_SOLID,3,RGB(250,196,88)); 
	pInsideBoundryPenRight.CreatePen(PS_INSIDEFRAME|PS_SOLID,3,RGB(251,202,106)); 
	pInsideBoundryPenTop.CreatePen(PS_INSIDEFRAME|PS_SOLID,2,RGB(252,210,121)); 
	pInsideBoundryPenBottom.CreatePen(PS_INSIDEFRAME|PS_SOLID,2,RGB(229,151,0)); 
	pFillActive.CreateSolidBrush(RGB(222,223,236));
	pFillInactive.CreateSolidBrush(RGB(222,223,236));
	m_bOverControl = m_bTracking = 0;
}

CXPButton::~CXPButton()
{
	// TODO: Cleanup your control's instance data here.
	pBoundryPen.DeleteObject();
	pFillActive.DeleteObject();
	pFillInactive.DeleteObject();
	pInsideBoundryPenLeft.DeleteObject();
	pInsideBoundryPenRight.DeleteObject();
	pInsideBoundryPenBottom.DeleteObject();
	pInsideBoundryPenTop.DeleteObject();
	
}


BEGIN_MESSAGE_MAP(CXPButton, CButton)
//{{AFX_MSG_MAP(CXPButton)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
ON_WM_MOUSEMOVE()
ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXPButton message handlers

void CXPButton::DoGradientFill(CDC *pDC, CRect rect)
{
	CBrush pBrush[64];
	int nWidth = (rect.right) - (rect.left);
    int nHeight = (rect.bottom) - (rect.top);
    CRect rct;
    for (int i=0; i<64; i++)
	{
		if(m_bOverControl)
		{
			if(Focus)
			{
				pBrush[i].CreateSolidBrush(RGB(255-(i/4), 255-(i/4), 255-(i/3)));
			}
			else
			{
				pBrush[i].CreateSolidBrush(RGB(255-(i/4), 255-(i/4), 255-(i/5)));			
			}
		}
		else
		{
			if(Focus)
			{
				pBrush[i].CreateSolidBrush(RGB(255-(i/3), 255-(i/3), 255-(i/4)));
			}
			else
			{
				pBrush[i].CreateSolidBrush(RGB(255-(i/3), 255-(i/3), 255-(i/5)));
			}
		}
	}
	
    for (int i=rect.top; i<nHeight+2; i++) 
	{
        rct.SetRect (rect.left, i, nWidth+2, i + 1);
        pDC->FillRect (&rct, &pBrush[((i * 63) / nHeight)]);
    }
	
    for (int i=0; i<64; i++)
	{
		pBrush[i].DeleteObject();
	}
	
}

void CXPButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC* pDC   = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rect = lpDrawItemStruct->rcItem;
    UINT state = lpDrawItemStruct->itemState;
	CPoint pt;
	CString strText;
	CString &m_title = strText;
	GetWindowText(strText);
	
    // draw the control edges (DrawFrameControl is handy!)
	
	pt.x = 10;
	pt.y = 10;
	
	pOldPen = pDC->SelectObject(&pBoundryPen);
	
	pDC->RoundRect(rect,pt);
	if(state & ODS_FOCUS)
	{
		Focus = TRUE;
	}
	else
	{
		Focus = FALSE;
	}
	
    if (state & ODS_SELECTED)
	{
		//pDC->DrawFrameControl(rect, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_PUSHED);
;		  
	}
    else
	{
		// pDC->DrawFrameControl(rect, DFC_BUTTON, DFCS_BUTTONPUSH);
;		
	}
    
    pDC->SelectObject(pOldPen);
	// Deflate the drawing rect by the size of the button's edges
    rect.DeflateRect( CSize(GetSystemMetrics(SM_CXEDGE), GetSystemMetrics(SM_CYEDGE)));
    // Fill the interior color if necessary
	
	if (m_bOverControl)
	{
		pOldBrush = pDC->SelectObject(&pFillActive);   
		//pDC->FloodFill(rect.left+5,rect.top+5,RGB(0,0,0));  //pDC->FillSolidRect(rect, RGB(255, 255, 0)); // yellow
		DoGradientFill(pDC,rect);
		DrawInsideBorder(pDC,rect);
	}
	else
	{
		pOldBrush = pDC->SelectObject(&pFillInactive);   
		//pDC->FloodFill(rect.left+5,rect.top+5,RGB(0,0,0));  //pDC->FillSolidRect(rect, RGB(0, 0, 255)); // yellow
		DoGradientFill(pDC,rect);
	}
	pDC->SelectObject(pOldBrush);
    
	// Draw the text
    if (!m_title.IsEmpty()/*!strText.IsEmpty()*/)
    {
        CSize Extent = pDC->GetTextExtent(m_title/*strText*/);
        CPoint pt( rect.CenterPoint().x - Extent.cx/2, 
			rect.CenterPoint().y - Extent.cy/2 );
		
        if (state & ODS_SELECTED) 
            pt.Offset(1,1);
		
        int nMode = pDC->SetBkMode(TRANSPARENT);
        //pOldPen = pDC->SelectObject(pBoundryPen);
        //CFont *pOldFont = SelectStockFont( pDC );
		
        if (state & ODS_DISABLED)
            pDC->DrawState(pt, Extent, m_title/*strCaption*/, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
        else
            pDC->TextOut(pt.x, pt.y, m_title/*strCaption*/);
        //pDC->SelectObject(pOldPen);
		//pDC->SelectObject(pOldFont);
        pDC->SetBkMode(nMode);
    }
	
	return;
	
}

void CXPButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0, BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

void CXPButton::DrawInsideBorder(CDC *pDC, CRect rect)
{
	CPoint oldpoint = pDC->MoveTo(rect.left,rect.bottom-3);
	pOldPen = pDC->SelectObject(&pInsideBoundryPenLeft);
	pDC->LineTo(rect.left,rect.top+2);
	pDC->SelectObject(&pInsideBoundryPenRight);
	pDC->MoveTo(rect.right-1,rect.bottom-3);
	pDC->LineTo(rect.right-1,rect.top+2);
	pDC->SelectObject(&pInsideBoundryPenTop);
	pDC->MoveTo(rect.left+2,rect.top);
	pDC->LineTo(rect.right-2,rect.top);
	pDC->SelectObject(&pInsideBoundryPenBottom);
	pDC->MoveTo(rect.left+2,rect.bottom);
	pDC->LineTo(rect.right-2,rect.bottom);
	pDC->SelectObject(pOldPen);	
	pDC->MoveTo(oldpoint);
}

void CXPButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);		
	}	
	CButton::OnMouseMove(nFlags, point);
	
}

LRESULT CXPButton::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	m_bOverControl=TRUE;
	Invalidate();
	return 0;
}

LRESULT CXPButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bOverControl = FALSE;
	Invalidate(FALSE);
	return 0;
}
