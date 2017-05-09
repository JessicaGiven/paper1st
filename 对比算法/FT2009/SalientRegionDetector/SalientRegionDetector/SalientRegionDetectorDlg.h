
// SalientRegionDetectorDlg.h : header file
//
#include <vector>
#include <string>
using namespace std;

#pragma once


// CSalientRegionDetectorDlg dialog
class CSalientRegionDetectorDlg : public CDialog
{
// Construction
public:
	CSalientRegionDetectorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SALIENTREGIONDETECTOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDetectSaliency();


private:

	void							GetPictures(vector<string>& picvec);
	bool							BrowseForFolder(string& folderpath);
};
