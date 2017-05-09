
// SalientRegionDetectorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SalientRegionDetector.h"
#include "SalientRegionDetectorDlg.h"
#include "PictureHandler.h"
#include "Saliency.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSalientRegionDetectorDlg dialog




CSalientRegionDetectorDlg::CSalientRegionDetectorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSalientRegionDetectorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSalientRegionDetectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSalientRegionDetectorDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_DETECTSALIENCY, &CSalientRegionDetectorDlg::OnBnClickedButtonDetectSaliency)
END_MESSAGE_MAP()


// CSalientRegionDetectorDlg message handlers

BOOL CSalientRegionDetectorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSalientRegionDetectorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSalientRegionDetectorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//=================================================================================
///	GetPictures
///
///	This function collects all the pictures the user chooses into a vector.
//=================================================================================
void CSalientRegionDetectorDlg::GetPictures(vector<string>& picvec)
{
	CFileDialog cfd(TRUE,NULL,NULL,OFN_OVERWRITEPROMPT,L"*.*|*.*|",NULL);
	cfd.m_ofn.Flags |= OFN_ALLOWMULTISELECT;

	//cfd.PostMessage(WM_COMMAND, 40964, NULL);
	
	CString strFileNames;
	cfd.m_ofn.lpstrFile = strFileNames.GetBuffer(2048);
	cfd.m_ofn.nMaxFile = 2048;

	BOOL bResult = cfd.DoModal() == IDOK ? TRUE : FALSE;
	strFileNames.ReleaseBuffer();

	//if(cfd.DoModal() == IDOK)
	if( bResult )
	{
		POSITION pos = cfd.GetStartPosition();
		while (pos) 
		{
			CString imgFile = cfd.GetNextPathName(pos);			
			PictureHandler ph;
			string name = ph.Wide2Narrow(imgFile.GetString());
			picvec.push_back(name);
		}
	}
	else return;
}

//===========================================================================
///	BrowseForFolder
//===========================================================================
bool CSalientRegionDetectorDlg::BrowseForFolder(string& folderpath)
{
	IMalloc* pMalloc = 0;
	if(::SHGetMalloc(&pMalloc) != NOERROR)
	return false;

	BROWSEINFO bi;
	memset(&bi, 0, sizeof(bi));

	bi.hwndOwner = m_hWnd;
	bi.lpszTitle = L"Please select a folder and press 'OK'.";

	LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);
	if(pIDL == NULL)
	return false;

	TCHAR buffer[_MAX_PATH];
	if(::SHGetPathFromIDList(pIDL, buffer) == 0)
	return false;
	PictureHandler pichand;
	folderpath = pichand.Wide2Narrow(buffer);
	folderpath.append("\\");
	return true;
}


//===========================================================================
///	OnBnClickedButtonDetectSaliency
///
///	The main function
//===========================================================================
void CSalientRegionDetectorDlg::OnBnClickedButtonDetectSaliency()
{
	PictureHandler picHand;
	vector<string> picvec(0);
	picvec.resize(0);
	GetPictures(picvec);//user chooses one or more pictures
	string saveLocation = "C:\\temp\\";
	BrowseForFolder(saveLocation);

	int numPics( picvec.size() );

	for( int k = 0; k < numPics; k++ )
	{
		vector<UINT> img(0);// or UINT* imgBuffer;
		int width(0);
		int height(0);

		picHand.GetPictureBuffer( picvec[k], img, width, height );
		int sz = width*height;

		Saliency sal;
		vector<double> salmap(0);
		sal.GetSaliencyMap(img, width, height, salmap, true);

		vector<UINT> outimg(sz);
		for( int i = 0; i < sz; i++ )
		{
			int val = int(salmap[i] + 0.5);
			outimg[i] = val << 16 | val << 8 | val;
		}
		
		picHand.SavePicture(outimg, width, height, picvec[k], saveLocation, 1, "_salmap.jpg");// 0 is for BMP and 1 for JPEG)
	}
	AfxMessageBox(L"Done!", 0, 0);
}
