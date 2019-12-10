#pragma once

class CUdpReceiveDlg : public CDialog
{
public:
	CUdpReceiveDlg(CWnd* pParent = NULL);	

	enum { IDD = IDD_UDPRECEIVE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnReceiveSock(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

private:
	SOCKET m_socket; 

	BOOL initSocket();
	BOOL SendMessage();	
};
