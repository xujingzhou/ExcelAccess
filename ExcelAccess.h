//////////////////////////////////////////////////////////////////////////////
//类名：CExcelAccessApp
//功能：应用程序的初始化
//作者：徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//组织：未来工作室(Future Studio)
//日期：2003.5.1
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_EXCELACCESS_H__F2806EC5_636E_11D0_AA5D_9B16791C83E6__INCLUDED_)
#define AFX_EXCELACCESS_H__F2806EC5_636E_11D0_AA5D_9B16791C83E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CExcelAccessApp:
// See ExcelAccess.cpp for the implementation of this class
//

class CExcelAccessApp : public CWinApp
{
public:
	CExcelAccessApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExcelAccessApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CExcelAccessApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXCELACCESS_H__F2806EC5_636E_11D0_AA5D_9B16791C83E6__INCLUDED_)
