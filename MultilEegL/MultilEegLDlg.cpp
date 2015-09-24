
// MultilEegLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultilEegL.h"
#include "MultilEegLDlg.h"
#include "afxdialogex.h" 
#include "emotiv.h"

// Global variables
EmoEngineEventHandle eEvent;
EmoStateHandle eState;
unsigned int userID;
float secs;
unsigned int datarate;
bool readytocollect;
int option;
int state;
unsigned int channelCount;
double data[128][22];

UINT EmotivDataThreadProc(LPVOID pParam);

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CMultilEegLDlg dialog




CMultilEegLDlg::CMultilEegLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMultilEegLDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//  m_State = _T("");
	//  m_strState = _T("");
}

void CMultilEegLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDSTATE, m_State);
	//  DDX_Control(pDX, IDSTATE, m_State);
	//  DDX_Control(pDX, IDSTATE, m_State);
	//  DDX_Text(pDX, IDSTATE, m_strState);
	DDX_Control(pDX, IDSTATE, m_State);
}

BEGIN_MESSAGE_MAP(CMultilEegLDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDSTART, &CMultilEegLDlg::OnStart)
END_MESSAGE_MAP()


// CMultilEegLDlg message handlers

BOOL CMultilEegLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	eEvent = EE_EmoEngineEventCreate();
	eState = EE_EmoStateCreate();
	userID = 0;
	secs = 1;
	datarate = 0;
	readytocollect = false;
	option = 0;
	state = 0;
	channelCount = sizeof(targetChannelList) / sizeof(EE_DataChannel_t);

	if (EE_EngineConnect() != EDK_OK)
		AfxMessageBox(_T("Error connect to Emotiv!"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMultilEegLDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMultilEegLDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMultilEegLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMultilEegLDlg::OnStart()
{
	AfxBeginThread(EmotivDataThreadProc, NULL);
}

UINT EmotivDataThreadProc(LPVOID pParam)
{
	CMultilEegLDlg *currDlg = (CMultilEegLDlg *) AfxGetApp()->GetMainWnd();
	DataHandle hData = EE_DataCreate();
	EE_DataSetBufferSizeInSec(secs);
	CString currState;

	int total = 0;
	int tsecond = 0;

	while (1)
	{
		state = EE_EngineGetNextEvent(eEvent);

		if (EDK_OK == state)
		{
			EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
			EE_EmoEngineEventGetUserId(eEvent, &userID);

			if (eventType == EE_UserAdded)
			{
				currState = "User added.";
				currDlg->m_State.SetWindowTextW(currState);
				EE_DataAcquisitionEnable(userID, true);
				readytocollect = true;
			}
		}

		if (readytocollect == true)
		{
			EE_DataUpdateHandle(0, hData);

			unsigned int nSamplesTaken = 0;
			EE_DataGetNumberOfSample(hData, &nSamplesTaken);

			if (nSamplesTaken != 0)
			{
				double **buffer = new double *[channelCount];
				for (int i = 0; i < channelCount; i++)
					buffer[i] = new double[nSamplesTaken];

				if (EDK_OK != EE_DataGetMultiChannels(hData, targetChannelList, channelCount, buffer, nSamplesTaken))
					continue;

				for (int j = 0; j < (int)nSamplesTaken; j++)
					for (int i = 0; i < channelCount; i++)
						data[total + j][i] = buffer[j][i];

				for (int i = 0; i < channelCount; i++)
					delete buffer[i];
				delete buffer;
			}

			total += nSamplesTaken;

			if (total == 128)
			{
				tsecond++;
				total = 0;
				currState.Format(_T("Windows taken %d"), tsecond);
				currDlg->m_State.SetWindowTextW(currState);
			}
		}

		Sleep(10);
	}

	return 0;
}
