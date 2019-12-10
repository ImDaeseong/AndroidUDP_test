#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		


class CUdpReceiveApp : public CWinAppEx
{
public:
	CUdpReceiveApp();

	public:
	virtual BOOL InitInstance();

protected:	
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CUdpReceiveApp theApp;