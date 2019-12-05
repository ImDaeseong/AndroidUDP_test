#include "stdafx.h"
#include "UDPSend.h"
#include "UDPSendDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CUDPSendDlg::CUDPSendDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUDPSendDlg::IDD, pParent)
{
}

void CUDPSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUDPSendDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CUDPSendDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);
	ShowWindow(SW_HIDE);
	MoveWindow(-3333, -3333, 1, 1);

	SetTimer(1, 1000, NULL);	

	return TRUE; 
}

void CUDPSendDlg::OnDestroy() 
{
	KillTimer(1);

	CDialog::OnDestroy();
}

void CUDPSendDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CUDPSendDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{   
		SendData();
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CUDPSendDlg::SendData()
{
	SOCKET m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  
	if(INVALID_SOCKET == m_socket)  
		return FALSE;  
	
	SOCKADDR_IN addr;  
	memset(&addr, 0, sizeof(addr));  
	addr.sin_family = AF_INET;  
	addr.sin_addr.S_un.S_addr = htonl(INADDR_BROADCAST);
	addr.sin_port = htons(DEFAULT_PORT);  
	BOOL bBoardcast = TRUE; 
	int len = sizeof(addr);

	if(SOCKET_ERROR == setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (char*)&bBoardcast, sizeof(bBoardcast)))  
		return FALSE;  
	
	int nNetTimeout=3000;
	setsockopt(m_socket,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int));


	//send messsage
	CString strSend = "test message test message test message test message test message";
	char buf[1024] = {0};	
	sprintf(buf, "%s", strSend);

	int nResult = sendto(m_socket, buf, sizeof(buf), 0, (LPSOCKADDR)&addr, sizeof(addr));  
	if(nResult < 0)
	{
		//send fail 
	}
	else if(nResult < strlen(buf))
	{
		//sending 
	}
	else
	{
		//send complete 
	}

	closesocket(m_socket);  

	return TRUE;
}