//////////////////////////////////////////////////////////////////////////////
//类名：CExcelAccessDlg
//功能：对Excel文件进行新建、读写、内部数据行、列、单元格进行
//      查询，插入，替换等操作，对已有文件进行手工添加的也可以
//      正确读取等的示例实现
//作者：徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//组织：未来工作室(Future Studio)
//日期：2003.5.1
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_EXCELACCESSDLG_H__F2806EC7_636E_11D0_AA5D_9B16791C83E6__INCLUDED_)
#define AFX_EXCELACCESSDLG_H__F2806EC7_636E_11D0_AA5D_9B16791C83E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xpbutton.h"					// XP风格按钮
#include "HoverEdit.h"                  // XP风格编辑框
#include "PPToolTip.h"					// 加入阴影工具提示
/////////////////////////////////////////////////////////////////////////////
// CExcelAccessDlg dialog

class CExcelAccessDlg : public CDialog
{
// Construction
public:
	CExcelAccessDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CExcelAccessDlg)
	enum { IDD = IDD_EXCELACCESS_DIALOG };
	CHoverEdit	m_edRow;
	CHoverEdit	m_edColumn;
	CXPButton	m_OK;
	CXPButton	m_Cancel;
	CXPButton	m_Write;
	CXPButton	m_Query;
	CXPButton	m_About;
	CButton	m_Check;
	CListBox	m_AccessList;
	CString	m_strRow;
	CString	m_strColumn;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExcelAccessDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	CPPToolTip m_tooltip;         // 定义阴影工具提示变量

	// Generated message map functions
	//{{AFX_MSG(CExcelAccessDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnWriteexcel();
	afx_msg void OnQuery();
	afx_msg void OnAbout();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXCELACCESSDLG_H__F2806EC7_636E_11D0_AA5D_9B16791C83E6__INCLUDED_)
