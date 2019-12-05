#pragma once

class CUDPSendDlg : public CDialog
{
public:
	CUDPSendDlg(CWnd* pParent = NULL);	

	enum { IDD = IDD_UDPSEND_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

private:
	BOOL SendData();
};
