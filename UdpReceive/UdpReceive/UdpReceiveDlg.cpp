#include "stdafx.h"
#include "UdpReceive.h"
#include "UdpReceiveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CUdpReceiveDlg::CUdpReceiveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdpReceiveDlg::IDD, pParent)
{
	m_AryInfo.RemoveAll();
}

void CUdpReceiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
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

	m_ListCtrl.DeleteAllItems(); 
	m_ListCtrl.InsertColumn(0, _T("타입"), LVCFMT_CENTER, 50, -1); 
    m_ListCtrl.InsertColumn(1, _T("패키지명"), LVCFMT_LEFT, 229, -1); 
	m_ListCtrl.InsertColumn(2, _T("타임(초)"), LVCFMT_LEFT, 100, -1);
	m_ListCtrl.InsertColumn(3, _T("업데이트시간"), LVCFMT_LEFT, 100, -1);

	
	if(initSocket())
	{
		SetTimer(1, 10000, NULL);	
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

	RemoveAllGameInfo();

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
		RemoveEnddate();		
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

			//HOSTENT* ptHost = gethostbyaddr((char*)&addrFrom.sin_addr, 4, AF_INET);
			//CString sMsg;
			//sMsg.Format("%s(%s) say %s", ptHost->h_name,	inet_ntoa(addrFrom.sin_addr), sabuf.buf);

			CString strValue;
			strValue.Format("%s", sabuf.buf);

			CString sType;
			CString strPackageName;
			int nSecond;

			CStringArray stDetailApp;
			if(GetSplitParams(strValue, stDetailApp))
			{
				sType = stDetailApp.GetAt(0);
				strPackageName	= stDetailApp.GetAt(1);
				nSecond = atoi(stDetailApp.GetAt(2));
				AddGameInfo(sType, strPackageName, nSecond);
			}
			break;
		}
	default:
		break;
	}

	return 0;
}

BOOL CUdpReceiveDlg::SendMessage()
{
	SOCKADDR_IN addrSrv = {0};
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(80);

	CString strHostName;
	HOSTENT* ptHost = gethostbyname(strHostName.GetBuffer(strHostName.GetLength() + 1));
	addrSrv.sin_addr.S_un.S_addr = *(DWORD*)ptHost->h_addr_list[0];

	WSABUF saBuf = {0};
	CString strSend = "10.0.2.2";
	saBuf.len = strSend.GetLength() + 1;
	saBuf.buf = strSend.GetBuffer(strSend.GetLength() + 1);
	DWORD dwSend = 0;

	if (SOCKET_ERROR == WSASendTo(m_socket, &saBuf, 1, &dwSend, 0, (SOCKADDR*)&addrSrv,	sizeof(SOCKADDR), NULL, NULL))
		return FALSE;	

	return TRUE;
}

void CUdpReceiveDlg::AddGameInfo(CString sType, CString strPackageName, int nSecond)
{
	int nIndex = 0;
	BOOL bFind = FALSE;
	int nCount = (int)m_AryInfo.GetSize();
	for(int i=0; i < nCount; i++)
	{
		ST_GAME_INFO_RESPONSE* pInfo = (ST_GAME_INFO_RESPONSE*)m_AryInfo.GetAt(i);
		if(pInfo->m_strPackageName == strPackageName)
		{
			nIndex = i;
			bFind = TRUE;
			break;
		}	
	}

	COleDateTime tNow = COleDateTime::GetCurrentTime();
	
	if(bFind)
	{
       	ST_GAME_INFO_RESPONSE* pInfo = new ST_GAME_INFO_RESPONSE;
		pInfo->m_sType = sType;
		pInfo->m_strPackageName = strPackageName;
		pInfo->m_nSecond = nSecond;
		pInfo->m_strCurrentTime = tNow;
		m_AryInfo.SetAt(nIndex, (LPVOID)pInfo);
	}
	else
	{
		ST_GAME_INFO_RESPONSE* pInfo = new ST_GAME_INFO_RESPONSE;
		pInfo->m_sType = sType;
		pInfo->m_strPackageName = strPackageName;
		pInfo->m_nSecond = nSecond;
		pInfo->m_strCurrentTime = tNow;
		m_AryInfo.Add(pInfo);
	}

	InsertGameInfo(bFind, sType, strPackageName, nSecond, tNow);
}

void CUdpReceiveDlg::RemoveGameInfo(CString strPackageName)
{
	int nCount = (int)m_AryInfo.GetSize();
	for(int i=0; i < nCount; i++)
	{
		ST_GAME_INFO_RESPONSE* pInfo = (ST_GAME_INFO_RESPONSE*)m_AryInfo.GetAt(i);
		if(pInfo->m_strPackageName == strPackageName)
		{
			delete pInfo;
			pInfo = NULL;
	    	m_AryInfo.RemoveAt(i); 
			break;
		}	
	}
}

void CUdpReceiveDlg::RemoveAllGameInfo()
{
	int nCount = (int)m_AryInfo.GetSize();
	for(int i=0; i < nCount; i++)
	{
		ST_GAME_INFO_RESPONSE* pInfo = (ST_GAME_INFO_RESPONSE*)m_AryInfo.GetAt(i);
		if(pInfo)delete pInfo;
		pInfo = NULL;
	}
	m_AryInfo.RemoveAll();
}

void CUdpReceiveDlg::RemoveEnddate()
{
	int nCount = (int)m_AryInfo.GetSize();
	for(int i=0; i < nCount; i++)
	{
		ST_GAME_INFO_RESPONSE* pInfo = (ST_GAME_INFO_RESPONSE*)m_AryInfo.GetAt(i);
		if(pInfo->m_sType == "e")
		{
			CString sMsg;
			sMsg.Format("%s:%d", pInfo->m_strPackageName, pInfo->m_nSecond);

			delete pInfo;
			pInfo = NULL;
	    	m_AryInfo.RemoveAt(i); 
			m_ListCtrl.DeleteItem(i);
			break;
		}
		else
		{
			COleDateTime tNow = COleDateTime::GetCurrentTime();	
			if (tNow >= pInfo->m_strCurrentTime)
			{
				COleDateTimeSpan ts = tNow - pInfo->m_strCurrentTime;
				if(ts.GetTotalSeconds() >= 30)//30초 이상 응답이 없는 경우
				{
					delete pInfo;
					pInfo = NULL;
	    			m_AryInfo.RemoveAt(i); 
					m_ListCtrl.DeleteItem(i);
					break;

				}

			}
		}
	}
}

BOOL CUdpReceiveDlg::GetSplitParams(CString strInput, CStringArray& strResultArr)
{
	BOOL bFind = FALSE;

	strResultArr.RemoveAll();
	int nCount = strInput.Find(_T(":"));
	int nLength = 0;	
	CString strPrefix, strNext;
	strNext = strInput;
	CString strTmp;
	while(nCount!=-1)
	{
		strPrefix = strNext.Left(nCount);
		strResultArr.Add(strPrefix);
		nLength = strNext.GetLength();
		strTmp = strNext.Right(nLength - nCount - 1);
		strNext = strTmp;
		nCount = strNext.Find(_T(":"));
	}
	if(strNext != "")strResultArr.Add(strNext);

	if(strResultArr.GetCount() == 3)
		bFind = TRUE;

	return bFind;
}

void CUdpReceiveDlg::InsertGameInfo(BOOL bFind, CString sType, CString strPackageName, int nSecond, COleDateTime tNow)
{
	CString strCurrentTime;
	strCurrentTime.Format("%.4d%.2d%.2d%.2d%.2d%.2d", tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond());

	if(bFind)
	{
		for(int i=0; i < m_ListCtrl.GetItemCount(); i++)
		{			
			if( strPackageName == m_ListCtrl.GetItemText(i, 1) )
			{
				CString strSecond;
				strSecond.Format("%d", nSecond);
				m_ListCtrl.SetItemText(i, 0, _T(sType));
				m_ListCtrl.SetItemText(i, 1, _T(strPackageName));
				m_ListCtrl.SetItemText(i, 2, _T(strSecond));
				m_ListCtrl.SetItemText(i, 3, _T(strCurrentTime));
			}		
		}
	}
	else
	{
		CString strSecond;
		strSecond.Format("%d", nSecond);
		int nListIndex = m_ListCtrl.GetItemCount();
		m_ListCtrl.InsertItem(nListIndex, _T(sType));
		m_ListCtrl.SetItemText(nListIndex, 1, _T(strPackageName));
		m_ListCtrl.SetItemText(nListIndex, 2, _T(strSecond));
		m_ListCtrl.SetItemText(nListIndex, 3, _T(strCurrentTime));
	}
}