
// FTPCrackerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include <afxinet.h>
#include <comdef.h>
#include <afxwin.h>
// CFTPCrackerDlg �Ի���
class CFTPCrackerDlg : public CDialogEx
{
// ����
public:
	CFTPCrackerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FTPCRACKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	int m_Mode;		//m_Mode=0 ��������λ������
	afx_msg void OnBnClickedCracke();
	afx_msg void OnBnClickedAllnum();
	afx_msg void OnBnClickedFourNumAlph();
	afx_msg void OnBnClickedEightNumAlph();
	BOOL ConnectFtp(CString strServer, CString strUser, CString strPass);
};
