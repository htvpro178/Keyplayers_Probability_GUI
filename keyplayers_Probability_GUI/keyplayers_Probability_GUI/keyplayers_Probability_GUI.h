
// keyplayers_Probability_GUI.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Ckeyplayers_Probability_GUIApp:
// See keyplayers_Probability_GUI.cpp for the implementation of this class
//

class Ckeyplayers_Probability_GUIApp : public CWinApp
{
public:
	Ckeyplayers_Probability_GUIApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Ckeyplayers_Probability_GUIApp theApp;