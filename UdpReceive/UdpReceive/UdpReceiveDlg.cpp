#include "stdafx.h"
#include "UdpReceive.h"
#include "UdpReceiveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CUdpReceiveDlg::CUdpReceiveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdpReceiveDlg::IDD, pParent)
{
}

void CUdpReceiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUdpReceiveDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(RECEIVE_SOCK, OnReceiveSock)
END_MESSAGE_MAP()

BOOL CUdpReceiveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	if(initSocket())
	{
		SetTimer(1, 1000, NULL);	
	}

	return TRUE;  
}


void CUdpReceiveDlg::OnDestroy() 
{
	KillTimer(1);

	if (m_socket)
	{
		closesocket(m_socket);
	}

	CDialog::OnDestroy();
}

void CUdpReceiveDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CUdpReceiveDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CUdpReceiveDlg::initSocket()
{
	m_socket = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, 0);
	if (m_socket == INVALID_SOCKET)
		return FALSE;
	
	sockaddr_in sockaddr;
	sockaddr.sin_addr.S_un.S_addr = INADDR_ANY;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(DEFAULT_PORT);
	
	if (bind(m_socket, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
		return FALSE;
	
	if (WSAAsyncSelect(m_socket, m_hWnd, RECEIVE_SOCK, FD_READ) == SOCKET_ERROR)
		return FALSE;
	
	return TRUE;
}

LRESULT CUdpReceiveDlg::OnReceiveSock(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(lParam))
	{
	case FD_READ:
		{
			WSABUF sabuf = {0};
			char strRecv[1024] = {0};
			sabuf.buf = strRecv;
			sabuf.len = 1024;
			DWORD dwRead;
			DWORD flag = 0;
			SOCKADDR_IN addrFrom;
			int len = sizeof(SOCKADDR);

			if (WSARecvFrom(m_socket, &sabuf, 1, &dwRead, &flag, (SOCKADDR*)&addrFrom, &len, NULL, NULL) == SOCKET_ERROR)
			{
				delete[] sabuf.buf;
				return FALSE;
			}

			HOSTENT* ptHost = gethostbyaddr((char*)&addrFrom.sin_addr, 4, AF_INET);
			CString sMsg;
			sMsg.Format("%s(%s) say %s", ptHost->h_name,	inet_ntoa(addrFrom.sin_addr), sabuf.buf);

			//SendMessage();

			break;
		}
	default:
		break;
	}

}

BOOL CUdpReceiveDlg::SendMessage()
{
	SOCKADDR_IN addrSrv = {0};
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(20121);

	CString strHostName;
	HOSTENT* ptHost = gethostbyname(strHostName.GetBuffer(strHostName.GetLength() + 1));
	addrSrv.sin_addr.S_un.S_addr = *(DWORD*)ptHost->h_addr_list[0];

	WSABUF saBuf = {0};
	CString strSend = "127.0.0.1";
	saBuf.len = strSend.GetLength() + 1;
	saBuf.buf = strSend.GetBuffer(strSend.GetLength() + 1);
	DWORD dwSend = 0;

	if (SOCKET_ERROR == WSASendTo(m_socket, &saBuf, 1, &dwSend, 0, (SOCKADDR*)&addrSrv,	sizeof(SOCKADDR), NULL, NULL))
		return FALSE;	

	return TRUE;
}