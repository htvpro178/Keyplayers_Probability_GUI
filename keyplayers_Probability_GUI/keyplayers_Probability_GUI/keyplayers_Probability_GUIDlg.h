
// keyplayers_Probability_GUIDlg.h : header file
//

#pragma once


// Ckeyplayers_Probability_GUIDlg dialog
class Ckeyplayers_Probability_GUIDlg : public CDialogEx
{
// Construction
public:
	Ckeyplayers_Probability_GUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_KEYPLAYERS_PROBABILITY_GUI_DIALOG };

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
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CString m_sFileIn;
	afx_msg void OnBnClickedBtnOpenFile();
	CString m_sVertex;
	CString m_sIteration;
	afx_msg void OnBnClickedButton1();
	BOOL ProcessFiles(CString sFileIn, int vertex, int sIteration, bool loadFile, bool flagAffect);
	afx_msg void OnBnClickedBtnAbout();
	afx_msg void OnBnClickedBtnAffect();
	afx_msg void OnBnClickedBtnReset();
};
