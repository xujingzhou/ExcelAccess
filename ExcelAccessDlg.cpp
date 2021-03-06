//////////////////////////////////////////////////////////////////////////////
//类名：CExcelAccessDlg
//功能：对Excel文件进行新建、读写、内部数据行、列、单元格进行
//      查询，插入，替换等操作，对已有文件进行手工添加的也可以
//      正确读取等的示例实现
//作者：徐景周(Johnny Xu, xujingzhou2016@gmail.com)
//组织：未来工作室(Future Studio)
//日期：2003.5.1
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ExcelAccess.h"
#include "ExcelAccessDlg.h"
#include "CSpreadSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CXPButton	m_OK;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDOK, m_OK);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExcelAccessDlg dialog

CExcelAccessDlg::CExcelAccessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExcelAccessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExcelAccessDlg)
	m_strRow = _T("");
	m_strColumn = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExcelAccessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExcelAccessDlg)
	DDX_Control(pDX, IDC_EDIT_ROW, m_edRow);
	DDX_Control(pDX, IDC_EDIT_COLUMN, m_edColumn);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, ID_WRITEEXCEL, m_Write);
	DDX_Control(pDX, ID_QUERY, m_Query);
	DDX_Control(pDX, ID_ABOUT, m_About);
	DDX_Control(pDX, IDC_CHECK1, m_Check);
	DDX_Control(pDX, IDC_LISTACCESS, m_AccessList);
	DDX_Text(pDX, IDC_EDIT_ROW, m_strRow);
	DDX_Text(pDX, IDC_EDIT_COLUMN, m_strColumn);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExcelAccessDlg, CDialog)
	//{{AFX_MSG_MAP(CExcelAccessDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_WRITEEXCEL, OnWriteexcel)
	ON_BN_CLICKED(ID_QUERY, OnQuery)
	ON_BN_CLICKED(ID_ABOUT, OnAbout)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExcelAccessDlg message handlers
//////////////////////////////////////////////////////////////////////////////
//名称：OnInitDialog
//功能：初始化对话框
//作者：徐景周(jingzhou_xu@163.net)
//组织：未来工作室(Future Studio)
//日期：2003.5.1
/////////////////////////////////////////////////////////////////////////////
BOOL CExcelAccessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// 创建阴影工具提示，并与相应提示控件联系起来
	m_tooltip.Create(this);
	m_tooltip.AddTool(GetDlgItem(IDOK), _T("<b><ct=0x0000FF><al_c>读取</b><br><ct=0x00AA00><hr=100%></ct><br>Excel表格数据"),IDI_PINFORMATION);
	m_tooltip.AddTool(GetDlgItem(ID_WRITEEXCEL), _T("<b><ct=0x0000FF><al_c>写入</b><br><ct=0x00AA00><hr=100%></ct><br>新建并写入Excel表格数据"),IDI_PINFORMATION);
	m_tooltip.AddTool(GetDlgItem(ID_ABOUT), _T("关于"));
	m_tooltip.AddTool(GetDlgItem(IDCANCEL), _T("退出"));
	m_tooltip.AddTool(GetDlgItem(ID_QUERY), _T("<b><ct=0x0000FF><al_c>查询</b><br><ct=0x00AA00><hr=100%></ct><br>Excel表格中行、列、单元格数据"),IDI_PINFORMATION);
	m_tooltip.AddTool(GetDlgItem(IDC_LISTACCESS), _T("<b><ct=0x0000FF><al_c>显示</b><br><ct=0x00AA00><hr=100%></ct><br>Excel表格数据"),IDI_PINFORMATION);
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK1), _T("<b><ct=0x0000FF><al_c>转换</b><br><ct=0x00AA00><hr=100%></ct><br>另存为其它文本格式文件"),IDI_PINFORMATION);
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_ROW), _T("<b><ct=0x0000FF><al_c>行号</b><br><ct=0x00AA00><hr=100%></ct><br>查询该行数据"),IDI_PINFORMATION);
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_COLUMN), _T("<b><ct=0x0000FF><al_c>列号</b><br><ct=0x00AA00><hr=100%></ct><br>查询该列数据"),IDI_PINFORMATION);

	// 显示图标或位图及字体属性
	m_tooltip.SetNotify();
	m_tooltip.SetDefaultFont();

	// 设置背景效果及渐变色
//	m_tooltip.SetColor(CPPToolTip::PPTOOLTIP_COLOR_BK_BEGIN, RGB(255, 255, 255));
//	m_tooltip.SetColor(CPPToolTip::PPTOOLTIP_COLOR_BK_MID,RGB(240, 247, 250));
//	m_tooltip.SetColor(CPPToolTip::PPTOOLTIP_COLOR_BK_END, RGB(192, 192, 200));
//	m_tooltip.SetEffectBk(14, 10);
	m_tooltip.SetColor(CPPToolTip::PPTOOLTIP_COLOR_BK_BEGIN, RGB(255, 255, 223));
	m_tooltip.SetColor(CPPToolTip::PPTOOLTIP_COLOR_BK_MID,RGB(192, 192, 172));
	m_tooltip.SetColor(CPPToolTip::PPTOOLTIP_COLOR_BK_END, RGB(128, 128, 112));
	m_tooltip.SetEffectBk(13, 10);

	// 多行显示提示信息
	m_tooltip.SetBehaviour(PPTOOLTIP_MULTIPLE_SHOW);


	// 程序初始启动时，窗体位于最前方
	SetWindowPos(&wndTopMost,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CExcelAccessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CExcelAccessDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CExcelAccessDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//////////////////////////////////////////////////////////////////////////////
//名称：OnOK
//功能：读取指定Excel文件内容
//作者：徐景周(jingzhou_xu@163.net)
//组织：未来工作室(Future Studio)
//日期：2003.5.1
///////////////////////////////////////////////////////////////////////
void CExcelAccessDlg::OnOK() 
{
	CSpreadSheet SS("c:\\Test.xls", "TestSheet");

	CStringArray Rows, Column;

	//清空列表框
	m_AccessList.ResetContent();
	for (int i = 1; i <= SS.GetTotalRows(); i++)
	{
		// 读取一行
		SS.ReadRow(Rows, i);
		CString strContents = "";
		for (int j = 1; j <= Rows.GetSize(); j++)
		{
			if(j == 1)
				strContents = Rows.GetAt(j-1);
			else
				strContents = strContents +  " --> " + Rows.GetAt(j-1);
		}

		m_AccessList.AddString(strContents);
	}
}

//////////////////////////////////////////////////////////////////////////////
//名称：OnWriteexcel
//功能：新建并写入Excel文件内容
//作者：徐景周(jingzhou_xu@163.net)
//组织：未来工作室(Future Studio)
//日期：2003.5.1
///////////////////////////////////////////////////////////////////////
void CExcelAccessDlg::OnWriteexcel() 
{
	// 新建Excel文件名及路径，TestSheet为内部表名
	CSpreadSheet SS("c:\\Test.xls", "TestSheet");

	CStringArray sampleArray, testRow;
	
	SS.BeginTransaction();
	
	// 加入标题
	sampleArray.RemoveAll();
	sampleArray.Add("姓名");
	sampleArray.Add("年龄");
	SS.AddHeaders(sampleArray);
	
	// 加入数据
	CString strName[] = {"徐景周","徐志慧","郭徽","牛英俊","朱小鹏"};
	CString strAge[]  = {"27","23","28","27","26"};
	for(int i = 0; i < sizeof(strName)/sizeof(CString); i++)
	{
		sampleArray.RemoveAll();
		sampleArray.Add(strName[i]);
		sampleArray.Add(strAge[i]);
		SS.AddRow(sampleArray);
	}
	
	// 初始化测试行数据，进行添加、插入及替换数据操作演示
	for (int k = 1; k <= 2; k++)
	{
		testRow.Add("Test");
	}
	
	SS.AddRow(testRow);				// 添加到尾部
	SS.AddRow(testRow, 2);			// 插入新行到第二行
	SS.AddRow(testRow, 6, true);	// 替换原第四行来新的内容
	
	SS.Commit();	
	
	if(m_Check.GetCheck())
		SS.Convert(";");            // 将原Excel文件转换为用分号分隔的文本，并另存为同名文本文件

	AfxMessageBox("文件写入成功!");
}

//////////////////////////////////////////////////////////////////////////////
//名称：OnQuery
//功能：查询指定Excel文件中行号、列号内容
//作者：徐景周(jingzhou_xu@163.net)
//组织：未来工作室(Future Studio)
//日期：2003.5.1
///////////////////////////////////////////////////////////////////////
void CExcelAccessDlg::OnQuery() 
{
	CSpreadSheet SS("c:\\Test.xls", "TestSheet");

	CStringArray Rows, Column;
	CString tempString = "";

	UpdateData();

	if(m_strRow == "" && m_strColumn == "")         // 查询为空
	{
		AfxMessageBox("行号、列号不能同时为空!");
		return;
	}    
	else if(m_strRow == "" && m_strColumn != "")    // 查询指定列数据
	{
		int iColumn = atoi(m_strColumn);
		int iCols = SS.GetTotalColumns();
		if(iColumn > iCols)							// 超出表范围查询时
		{
			CString str;
			str.Format("表中总列数为: %d, ", iCols);
			AfxMessageBox(str + " 查询列数大于Excel表中总列数，请重新输入!");
			return;
		}

		// 读取一列数据，并按行读出
		if(!SS.ReadColumn(Column, iColumn))
		{
			AfxMessageBox(SS.GetLastError());
			return;
		}

		CString tmpStr;
		for (int i = 0; i < Column.GetSize(); i++)
		{
			tmpStr.Format("行号: %d, 列号: %d ,内容: %s\n", i+1,iColumn,Column.GetAt(i));
			tempString += tmpStr;
		}
		
		AfxMessageBox(tempString);
	}
	else if(m_strRow != "" && m_strColumn == "")     // 查询指定行数数据
	{
		int iRow = atoi(m_strRow);
		int iRows = SS.GetTotalRows();
		
		if(iRow > iRows)							// 超出表范围查询时
		{
			CString str;
			str.Format("表中总行数为: %d, ", iRows);
			AfxMessageBox(str + " 查询行数大于Excel表中总行数，请重新输入!");
			return;
		}

		// 读取指定行数据
		if(!SS.ReadRow(Rows, iRow))
		{
			AfxMessageBox(SS.GetLastError());
			return;
		}

		CString tmpStr;
		for (int i = 0; i < Rows.GetSize(); i++)
		{
			tmpStr.Format("行号: %d, 列号: %d ,内容: %s\n", iRow, i+1, Rows.GetAt(i));
			tempString += tmpStr;
		}

		AfxMessageBox(tempString);
	}
	else if(m_strRow != "" && m_strColumn != "")     // 查询指定单元格数据
	{
		int iRow = atoi(m_strRow), iColumn = atoi(m_strColumn);
		int iRows = SS.GetTotalRows(), iCols = SS.GetTotalColumns(); 
		
		if(iColumn > iCols)                          // 超出表范围查询时
		{
			CString str;
			str.Format("表中总列数为: %d, ", iCols);
			AfxMessageBox(str + " 查询列数大于Excel表中总列数，请重新输入!");
			return;
		}
		else if(iRow > iRows)
		{
			CString str;
			str.Format("表中总行数为: %d, ", iRows);
			AfxMessageBox(str + " 查询行数大于Excel表中总行数，请重新输入!");
			return;
		}

		// 读取指定行、列单元格数据
		if(!SS.ReadCell(tempString, iColumn, iRow))
		{
			AfxMessageBox(SS.GetLastError());
			return;
		}

		CString str;
		str.Format("行号: %d, 列号: %d ,内容: %s", iRow,iColumn,tempString);
		AfxMessageBox(str);
	}
	
}

//////////////////////////////////////////////////////////////////////////////
//名称：OnAbout
//功能：关于对话框
//作者：徐景周(jingzhou_xu@163.net)
//组织：未来工作室(Future Studio)
//日期：2003.5.1
///////////////////////////////////////////////////////////////////////
void CExcelAccessDlg::OnAbout() 
{
	CAboutDlg dlg;
	dlg.DoModal();
}

//////////////////////////////////////////////////////////////////////////////
//名称：OnCtlColor
//功能：设置各控件前景、背景色
//作者：徐景周(jingzhou_xu@163.net)
//组织：未来工作室(Future Studio)
//日期：2003.5.1
/////////////////////////////////////////////////////////////////////////////
HBRUSH CExcelAccessDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor==CTLCOLOR_LISTBOX)
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(233,233,220));
		HBRUSH b=CreateSolidBrush(RGB(233,233,220));
		return b;
	}
	else if(nCtlColor==CTLCOLOR_SCROLLBAR)
	{
		
	
	}
	else if(nCtlColor==CTLCOLOR_EDIT)
	{
		
		
	}
	else if(nCtlColor==CTLCOLOR_STATIC)
	{
	
		
	}
	else if(nCtlColor==CTLCOLOR_DLG)
	{
		
		
	}
	
	return hbr;
}

//////////////////////////////////////////////////////////////////////////////
//名称：PreTranslateMessage
//功能：阴影工具提示的消息传递
//作者：徐景周(jingzhou_xu@163.net)
//组织：未来工作室(Future Studio)
//日期：2003.5.1
/////////////////////////////////////////////////////////////////////////////
BOOL CExcelAccessDlg::PreTranslateMessage(MSG* pMsg) 
{
	// 为阴影工具提示加入鼠标事件传递
	m_tooltip.RelayEvent(pMsg);	

	return CDialog::PreTranslateMessage(pMsg);
}
