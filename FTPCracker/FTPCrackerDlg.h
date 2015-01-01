
// FTPCrackerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include <afxinet.h>
#include <comdef.h>
#include <afxwin.h>
// CFTPCrackerDlg 对话框
class CFTPCrackerDlg : public CDialogEx
{
// 构造
public:
	CFTPCrackerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FTPCRACKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_ServerName;
	CString m_UserName;
	CString m_Result;
	CString m_Time;
	int m_Mode;		//m_Mode=0 不大于四位纯数字
	afx_msg void OnBnClickedCracke();
	afx_msg void OnBnClickedAllnum();
	afx_msg void OnBnClickedFourNumAlph();
	afx_msg void OnBnClickedEightNumAlph();
	BOOL ConnectFtp(CString strServer, CString strUser, CString strPass);
};
