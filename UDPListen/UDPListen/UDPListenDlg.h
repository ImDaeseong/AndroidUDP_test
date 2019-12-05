#pragma once

struct RECVPARAM{
	SOCKET socket;
	HWND hWnd;
};

class CUDPListenDlg : public CDialog
{
public:
	CUDPListenDlg(CWnd* pParent = NULL);	

	enum { IDD = IDD_UDPLISTEN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

private:
	SOCKET m_socket; 

	BOOL initSocket();	
	void TimerReceiveData();

	static DWORD WINAPI ThreadReceiveData(LPVOID pParam);
};
