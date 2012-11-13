#ifndef MainWnd_h__
#define MainWnd_h__

#include <map>
#include <ShellAPI.h>
#include   <time.h>
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

	bool OnMsgBtnMouseEnter(TEventUI* pTEventUI);
	bool OnMsgBtnMouseLeave(TEventUI* pTEventUI);
	bool OnMsgBtnClick(TNotifyUI* pTNotifyUI);
	bool OnEffectsBtnClick(TNotifyUI* pTNotifyUI);
	bool OnAddNodeBtnClick(TNotifyUI* pTNotifyUI);
	bool OnAddAtNodeBtnClick(TNotifyUI* pTNotifyUI);
	bool OnRemoveNodeBtnClick(TNotifyUI* pTNotifyUI);
	bool OnCreateChartViewBtn(TNotifyUI* pTNotifyUI);
	bool OnAddChartDataABtn(TNotifyUI* pTNotifyUI);
	bool OnAddChartDataBBtn(TNotifyUI* pTNotifyUI);
	bool OnAddChartDataCBtn(TNotifyUI* pTNotifyUI);
	bool OnLegendTopBtn(TNotifyUI* pTNotifyUI);
	bool OnLegendRightBtn(TNotifyUI* pTNotifyUI);
	bool OnLegendBottomBtn(TNotifyUI* pTNotifyUI);
	bool OnDelChartDataBtn(TNotifyUI* pTNotifyUI);

private:
	int				mChartDataPos;
	CLabelUI*		pTestLabel;
	CEditUI*		pTestEdit;
	CButtonUI*		pEffectsDemo;
	CChartViewUI*	pChartView;
	CHorizontalLayoutUI* pAnimWnd;
	HMENU			hMenu;
	NOTIFYICONDATA	nid;

};

#endif // MainWnd_h__
