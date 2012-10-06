#ifndef MainWnd_h__
#define MainWnd_h__

#include <map>
#include <ShellAPI.h>
#include "../../Common/Include/UiLib/Utils/IWindowBase.h"

#pragma once
class CMainWnd : public IWindowBase
{
public:
	CMainWnd(void);
	~CMainWnd(void);

public:
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName() const ;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Init();
	void Notify(TNotifyUI& msg);
	void OnFinalMessage(HWND hWnd);

private:
	CLabelUI*		pTestLabel;
	CEditUI*		pTestEdit;
	CButtonUI*		pEffectsDemo;
	CHorizontalLayoutUI* pAnimWnd;
	HMENU			hMenu;
	NOTIFYICONDATA	nid;

};

#endif // MainWnd_h__
