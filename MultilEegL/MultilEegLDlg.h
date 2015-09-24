
// MultilEegLDlg.h : header file
//

#pragma once


// CMultilEegLDlg dialog
class CMultilEegLDlg : public CDialogEx
{
// Construction
public:
	CMultilEegLDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MULTILEEGL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStart();
//	CString m_State;
//	CEdit m_State;
//	CStatic m_State;
//	CString m_strState;
	CStatic m_State;
};
