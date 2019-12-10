#include "stdafx.h"
#include "UDPListen.h"
#include "UDPListenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CUDPListenDlg::CUDPListenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUDPListenDlg::IDD, pParent)
{	
	m_socket = NULL;
}

void CUDPListenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUDPListenDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CUDPListenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);
	ShowWindow(SW_HIDE);
	MoveWindow(-3333, -3333, 1, 1);
		
	if(initSocket())
	{
		SetTimer(1, 1000, NULL);	
	}

	return TRUE;  
}

void CUDPListenDlg::OnDestroy() 
{
	KillTimer(1);

	if (m_socket)
	{
		closesocket(m_socket);
	}

	CDialog::OnDestroy();
}

void CUDPListenDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CUDPListenDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		//TimerReceiveData();
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CUDPListenDlg::initSocket()
{
	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_socket == INVALID_SOCKET)
		return FALSE;


	SOCKADDR_IN localAddr;
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(DEFAULT_PORT);
	localAddr.sin_addr.s_addr = INADDR_ANY;

	BOOL bBoardcast = TRUE; 
	setsockopt(m_socket,SOL_SOCKET,SO_BROADCAST,(const char *)&bBoardcast,sizeof(bBoardcast));
	
	int nResult = bind(m_socket, (struct sockaddr*)&localAddr, sizeof(SOCKADDR_IN));
	if (nResult < 0) 
		return FALSE;

	RECVPARAM* param = new RECVPARAM;
	param->socket = m_socket;
	param->hWnd = this->m_hWnd;
	HANDLE hThread = CreateThread(NULL, NULL, ThreadReceiveData, (LPVOID)param, NULL, NULL);
	CloseHandle(hThread);

	return TRUE;
}

void CUDPListenDlg::TimerReceiveData()
{
	try
	{
		int nResult = 0;
		int nLength = sizeof(SOCKADDR);
		
		char buff[1024] = {0,};
		
		SOCKADDR_IN localAddr;	
		localAddr.sin_family = AF_INET;
		localAddr.sin_addr.s_addr = INADDR_BROADCAST;
		localAddr.sin_port = htons(DEFAULT_PORT);

		memset(buff, 0, 1024);
		nResult = recvfrom(m_socket, buff, 1024, 0, ( SOCKADDR FAR*)&localAddr,(int far*) &nLength);
		if (nResult > 0)
		{
			CString sMsg;
			sMsg.Format("%s", buff);
		}	
	}
	catch (...)
	{	
	}	
}

DWORD WINAPI CUDPListenDlg::ThreadReceiveData(LPVOID pParam)
{	
	try
	{
		RECVPARAM* param = (RECVPARAM*)pParam;
		SOCKET socket = param->socket;
		HWND HwndRecv = param->hWnd;
		delete param;


		int nResult = 0;
		int nLength = sizeof(SOCKADDR);
		
		char buff[1024] = {0,};
		
		SOCKADDR_IN localAddr;	
		localAddr.sin_family = AF_INET;
		localAddr.sin_addr.s_addr = INADDR_BROADCAST;
		localAddr.sin_port = htons(DEFAULT_PORT);

		while (true)
		{		
			memset(buff, 0, 1024);
			nResult = recvfrom(socket, buff, 1024, 0, (SOCKADDR*)&localAddr, &nLength);
			if (nResult > 0)
			{
				char* chIP = inet_ntoa(localAddr.sin_addr);
				CString sMsg;
				sMsg.Format("%s - %s", chIP, buff);
			}		
		}
	}
	catch (...)
	{	
	}

	return 0;	
}
