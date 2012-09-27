#include "StdAfx.h"
#include "UIWebBrowser.h"
#include <atlconv.h>

UiLib::CWebBrowserUI::CWebBrowserUI()
	: m_pWebBrowser2(NULL)
	, m_pWebBrowserEventHandler(NULL)
	, m_bAutoNavi(false)
{
	m_clsid=CLSID_WebBrowser;
	m_sHomePage.Empty();
}

bool UiLib::CWebBrowserUI::DoCreateControl()
{
	if (!CActiveXUI::DoCreateControl())
		return false;
	m_pManager->AddTranslateAccelerator(this);
	SetDispatchHandler(this);
	SetExternalUIHandler(this);
	SetDownloadManager(this);
	GetControl(IID_IWebBrowser2,(LPVOID*)&m_pWebBrowser2);
	if ( m_bAutoNavi && !m_sHomePage.IsEmpty())
	{
		this->Navigate2(m_sHomePage);
	}
	return true;
}

void UiLib::CWebBrowserUI::ReleaseControl()
{
	m_bCreated=false;
	m_pManager->RemoveTranslateAccelerator(this);
	CActiveXUI::ReleaseControl();
}

UiLib::CWebBrowserUI::~CWebBrowserUI()
{

}

STDMETHODIMP UiLib::CWebBrowserUI::GetTypeInfoCount( UINT *iTInfo )
{
	*iTInfo = 0;
	return S_OK;
}

STDMETHODIMP UiLib::CWebBrowserUI::GetTypeInfo( UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo )
{
	return E_NOTIMPL;
}

STDMETHODIMP UiLib::CWebBrowserUI::GetIDsOfNames( REFIID riid, OLECHAR **rgszNames, UINT cNames, LCID lcid,DISPID *rgDispId )
{
	return E_NOTIMPL;
}

STDMETHODIMP UiLib::CWebBrowserUI::Invoke( DISPID dispIdMember, REFIID riid, LCID lcid,WORD wFlags, DISPPARAMS* pDispParams,VARIANT* pVarResult, EXCEPINFO* pExcepInfo,UINT* puArgErr )
{
	if ((riid != IID_NULL))
		return E_INVALIDARG;

	switch(dispIdMember)
	{
	case DISPID_BEFORENAVIGATE2:
		BeforeNavigate2(
			pDispParams->rgvarg[6].pdispVal,
			pDispParams->rgvarg[5].pvarVal,
			pDispParams->rgvarg[4].pvarVal,
			pDispParams->rgvarg[3].pvarVal,
			pDispParams->rgvarg[2].pvarVal,
			pDispParams->rgvarg[1].pvarVal,
			pDispParams->rgvarg[0].pboolVal);
		break;
	case DISPID_COMMANDSTATECHANGE:
		CommandStateChange(
			pDispParams->rgvarg[1].lVal,
			pDispParams->rgvarg[0].boolVal);
		break;
	case DISPID_NAVIGATECOMPLETE2:
		NavigateComplete2(
			pDispParams->rgvarg[1].pdispVal,
			pDispParams->rgvarg[0].pvarVal);
		break;
	case DISPID_NAVIGATEERROR:
		NavigateError(
			pDispParams->rgvarg[4].pdispVal,
			pDispParams->rgvarg[3].pvarVal,
			pDispParams->rgvarg[2].pvarVal,
			pDispParams->rgvarg[1].pvarVal,
			pDispParams->rgvarg[0].pboolVal);
		break;
	case DISPID_STATUSTEXTCHANGE:
		break;
		//      case DISPID_NEWWINDOW2:
		//              break;
	case DISPID_NEWWINDOW3:
		NewWindow3(
			pDispParams->rgvarg[4].ppdispVal,
			pDispParams->rgvarg[3].pboolVal,
			pDispParams->rgvarg[2].uintVal,
			pDispParams->rgvarg[1].bstrVal,
			pDispParams->rgvarg[0].bstrVal);
		break;
		//      case DISPID_PROPERTYCHANGE:
		//              if (pDispParams->cArgs>0 && pDispParams->rgvarg[0].vt == VT_BSTR) {
		//                      TRACE(_T("PropertyChange(%s)\n"), pDispParams->rgvarg[0].bstrVal);
		//              }
		//              break;
	default:
		return E_NOTIMPL;
	}
	return S_OK;
}

STDMETHODIMP UiLib::CWebBrowserUI::QueryInterface( REFIID riid, LPVOID *ppvObject )
{
	return S_OK;
}

STDMETHODIMP_(ULONG) UiLib::CWebBrowserUI::AddRef()
{
	InterlockedIncrement(&m_dwRef);
	return m_dwRef;
}

STDMETHODIMP_(ULONG) UiLib::CWebBrowserUI::Release()
{
	ULONG ulRefCount = InterlockedDecrement(&m_dwRef);
	return ulRefCount;
}

void UiLib::CWebBrowserUI::Navigate2( LPCTSTR lpszUrl )
{
	if (lpszUrl == NULL)
		return;

	if (m_pWebBrowser2)
	{
		VARIANT v;
		v.vt=VT_BSTR;
		v.bstrVal=T2BSTR(lpszUrl);
		HRESULT hr = m_pWebBrowser2->Navigate2(&v, NULL, NULL, NULL, NULL);
	}
}

void UiLib::CWebBrowserUI::Refresh()
{
	if (m_pWebBrowser2)
	{
		m_pWebBrowser2->Refresh();
	}
}
/// DWebBrowserEvents2
void UiLib::CWebBrowserUI::BeforeNavigate2( IDispatch *pDisp,VARIANT *&url,VARIANT *&Flags,VARIANT *&TargetFrameName,VARIANT *&PostData,VARIANT *&Headers,VARIANT_BOOL *&Cancel )
{
	if (m_pWebBrowserEventHandler)
	{
		m_pWebBrowserEventHandler->BeforeNavigate2(pDisp,url,Flags,TargetFrameName,PostData,Headers,Cancel);
	}
}

void UiLib::CWebBrowserUI::NavigateError( IDispatch *pDisp,VARIANT * &url,VARIANT *&TargetFrameName,VARIANT *&StatusCode,VARIANT_BOOL *&Cancel )
{
	if (m_pWebBrowserEventHandler)
	{
		m_pWebBrowserEventHandler->NavigateError(pDisp,url,TargetFrameName,StatusCode,Cancel);
	}
}

void UiLib::CWebBrowserUI::NavigateComplete2( IDispatch *pDisp,VARIANT *&url )
{
	if (m_pWebBrowserEventHandler)
	{
		m_pWebBrowserEventHandler->NavigateComplete2(pDisp,url);
	}
}

void UiLib::CWebBrowserUI::ProgressChange( LONG nProgress, LONG nProgressMax )
{
	if (m_pWebBrowserEventHandler)
	{
		m_pWebBrowserEventHandler->ProgressChange(nProgress,nProgressMax);
	}
}

void UiLib::CWebBrowserUI::NewWindow3( IDispatch **pDisp, VARIANT_BOOL *&Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl )
{
	if (m_pWebBrowserEventHandler)
	{
		m_pWebBrowserEventHandler->NewWindow3(pDisp,Cancel,dwFlags,bstrUrlContext,bstrUrl);
	}
}
void UiLib::CWebBrowserUI::CommandStateChange(long Command,VARIANT_BOOL Enable)
{
	if (m_pWebBrowserEventHandler)
	{
		m_pWebBrowserEventHandler->CommandStateChange(Command,Enable);
	}
}

// IDownloadManager
STDMETHODIMP UiLib::CWebBrowserUI::Download( /* [in] */ IMoniker *pmk, /* [in] */ IBindCtx *pbc, /* [in] */ DWORD dwBindVerb, /* [in] */ LONG grfBINDF, /* [in] */ BINDINFO *pBindInfo, /* [in] */ LPCOLESTR pszHeaders, /* [in] */ LPCOLESTR pszRedir, /* [in] */ UINT uiCP )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->Download(pmk,pbc,dwBindVerb,grfBINDF,pBindInfo,pszHeaders,pszRedir,uiCP);
	}
	return S_OK;
}

// IDocHostUIHandler
STDMETHODIMP UiLib::CWebBrowserUI::ShowContextMenu( DWORD dwID, POINT* pptPosition, IUnknown* pCommandTarget, IDispatch* pDispatchObjectHit )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->ShowContextMenu(dwID,pptPosition,pCommandTarget,pDispatchObjectHit);
	}
	return S_OK;
}

STDMETHODIMP UiLib::CWebBrowserUI::GetHostInfo( DOCHOSTUIINFO* pInfo )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->GetHostInfo(pInfo);
	}
	return S_OK;
}

STDMETHODIMP UiLib::CWebBrowserUI::ShowUI( DWORD dwID, IOleInPlaceActiveObject* pActiveObject, IOleCommandTarget* pCommandTarget, IOleInPlaceFrame* pFrame, IOleInPlaceUIWindow* pDoc )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->ShowUI(dwID,pActiveObject,pCommandTarget,pFrame,pDoc);
	}
	return S_OK;
}

STDMETHODIMP UiLib::CWebBrowserUI::HideUI()
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->HideUI();
	}
	return S_OK;
}

STDMETHODIMP UiLib::CWebBrowserUI::UpdateUI()
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->UpdateUI();
	}
	return S_OK;
}

STDMETHODIMP UiLib::CWebBrowserUI::EnableModeless( BOOL fEnable )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->EnableModeless(fEnable);
	}
	return S_OK;
}

STDMETHODIMP UiLib::CWebBrowserUI::OnDocWindowActivate( BOOL fActivate )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->OnDocWindowActivate(fActivate);
	}
	return S_OK;
}

STDMETHODIMP UiLib::CWebBrowserUI::OnFrameWindowActivate( BOOL fActivate )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->OnFrameWindowActivate(fActivate);
	}
	return S_OK;
}

STDMETHODIMP UiLib::CWebBrowserUI::ResizeBorder( LPCRECT prcBorder, IOleInPlaceUIWindow* pUIWindow, BOOL fFrameWindow )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->ResizeBorder(prcBorder,pUIWindow,fFrameWindow);
	}
	return S_OK;
}

STDMETHODIMP UiLib::CWebBrowserUI::TranslateAccelerator( LPMSG lpMsg, const GUID* pguidCmdGroup, DWORD nCmdID )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->TranslateAccelerator(lpMsg,pguidCmdGroup,nCmdID);
	}
	return S_OK;
}

LRESULT UiLib::CWebBrowserUI::TranslateAccelerator( MSG *pMsg )
{
	if( m_pWebBrowser2 != NULL )
	{
		IOleInPlaceActiveObject *pObj;
		if (FAILED(m_pWebBrowser2->QueryInterface(IID_IOleInPlaceActiveObject, (LPVOID *)&pObj)))
			return S_FALSE;
		return pObj->TranslateAccelerator(pMsg);
	}
	return E_NOTIMPL;
}

STDMETHODIMP UiLib::CWebBrowserUI::GetOptionKeyPath( LPOLESTR* pchKey, DWORD dwReserved )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->GetOptionKeyPath(pchKey,dwReserved);
	}
	return S_OK;
}

STDMETHODIMP UiLib::CWebBrowserUI::GetDropTarget( IDropTarget* pDropTarget, IDropTarget** ppDropTarget )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->GetDropTarget(pDropTarget,ppDropTarget);
	}
	return S_OK;
}

STDMETHODIMP UiLib::CWebBrowserUI::GetExternal( IDispatch** ppDispatch )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->GetExternal(ppDispatch);
	}
	return S_OK;
}

STDMETHODIMP UiLib::CWebBrowserUI::TranslateUrl( DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->TranslateUrl(dwTranslate,pchURLIn,ppchURLOut);
	}
	return S_OK;
}

STDMETHODIMP UiLib::CWebBrowserUI::FilterDataObject( IDataObject* pDO, IDataObject** ppDORet )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->FilterDataObject(pDO,ppDORet);
	}
	return S_OK;
}

void UiLib::CWebBrowserUI::SetWebBrowserEventHandler( CWebBrowserEventHandler* pEventHandler )
{
	if ( pEventHandler!=NULL && m_pWebBrowserEventHandler!=pEventHandler)
		m_pWebBrowserEventHandler=pEventHandler;
}

void UiLib::CWebBrowserUI::Refresh2( int Level )
{
	VARIANT vLevel;
	VariantInit(&vLevel);
	vLevel.vt=VT_I4;
	vLevel.intVal=Level;
	m_pWebBrowser2->Refresh2(&vLevel);
	VariantClear(&vLevel);
}

void UiLib::CWebBrowserUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	if (_tcscmp(pstrName, _T("homepage")) == 0)
	{
		m_sHomePage = pstrValue;
	}
	else if (_tcscmp(pstrName, _T("autonavi"))==0)
	{
		m_bAutoNavi = (_tcscmp(pstrValue, _T("true")) == 0);
	}
	else
		CActiveXUI::SetAttribute(pstrName, pstrValue);
}

void UiLib::CWebBrowserUI::NavigateHomePage()
{
	if (!m_sHomePage.IsEmpty())
		this->NavigateUrl(m_sHomePage);
}

void UiLib::CWebBrowserUI::NavigateUrl( LPCTSTR lpszUrl )
{
	if (m_pWebBrowser2 && lpszUrl)
	{
		m_pWebBrowser2->Navigate((BSTR)SysAllocString(T2BSTR(lpszUrl)),NULL,NULL,NULL,NULL);
	}
}

LPCTSTR UiLib::CWebBrowserUI::GetClass() const
{
	return _T("WebBrowserUI");
}

LPVOID UiLib::CWebBrowserUI::GetInterface( LPCTSTR pstrName )
{
	if( _tcscmp(pstrName, _T("WebBrowser")) == 0 ) return static_cast<CWebBrowserUI*>(this);
	return CActiveXUI::GetInterface(pstrName);
}

void UiLib::CWebBrowserUI::SetHomePage( LPCTSTR lpszUrl )
{
	m_sHomePage.Format(_T("%s"),lpszUrl);
}

LPCTSTR UiLib::CWebBrowserUI::GetHomePage()
{
	return m_sHomePage;
}

void UiLib::CWebBrowserUI::SetAutoNavigation( bool bAuto /*= TRUE*/ )
{
	if (m_bAutoNavi==bAuto) return;

	m_bAutoNavi=bAuto;
}

bool UiLib::CWebBrowserUI::IsAutoNavigation()
{
	return m_bAutoNavi;
}