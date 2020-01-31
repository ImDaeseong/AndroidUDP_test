#pragma once
#include "afxcmn.h"

typedef struct _Game_info_Response_
{
	CString m_sType;
	CString m_strPackageName;
	int m_nSecond;
	COleDateTime m_strCurrentTime;

}ST_GAME_INFO_RESPONSE, *LPST_GAME_INFO_RESPONSE;

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
	CListCtrl m_ListCtrl;


	CPtrArray m_AryInfo;
	void AddGameInfo(CString sType, CString strPackageName, int nSecond);
	void RemoveGameInfo(CString strPackageName);
	void RemoveAllGameInfo();
	void RemoveEnddate();
	BOOL GetSplitParams(CString strInput, CStringArray& strResultArr);
	void InsertGameInfo(BOOL bFind, CString sType, CString strPackageName, int nSecond, COleDateTime tNow);

	SOCKET m_socket; 
	BOOL initSocket();
	BOOL SendMessage();	
	
};
