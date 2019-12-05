#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		


class CUDPListenApp : public CWinAppEx
{
public:
	CUDPListenApp();

	public:
	virtual BOOL InitInstance();

protected:	
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CUDPListenApp theApp;