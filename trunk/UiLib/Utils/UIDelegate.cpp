#include "StdAfx.h"

namespace UiLib {


	//************************************
	// 函数名称: CDelegateBase
	// 返回类型: 
	// 参数信息: void * pObject
	// 参数信息: void * pFn
	// 函数说明: 
	//************************************
	CDelegateBase::CDelegateBase( void* pObject, void* pFn )
	{
		m_pObject = pObject;
		m_pFn = pFn;
		m_iEventType = UIEVENT__ALL;
		m_sNotifyTypeName.Empty();
	}

	//************************************
	// 函数名称: CDelegateBase
	// 返回类型: 
	// 参数信息: void * pObject
	// 参数信息: void * pFn
	// 参数信息: EVENTTYPE_UI _iEventType
	// 函数说明: 
	//************************************
	CDelegateBase::CDelegateBase( void* pObject, void* pFn,EVENTTYPE_UI _iEventType /*= UIEVENT__ALL*/ )
	{
		m_pObject = pObject;
		m_pFn = pFn;
		m_iEventType = _iEventType;
		m_sNotifyTypeName.Empty();
	}

	//************************************
	// 函数名称: CDelegateBase
	// 返回类型: 
	// 参数信息: void * pObject
	// 参数信息: void * pFn
	// 参数信息: LPCTSTR _sNotifyTypeName
	// 函数说明: 
	//************************************
	CDelegateBase::CDelegateBase( void* pObject, void* pFn,LPCTSTR _sNotifyTypeName /*= NULL*/ )
	{
		m_pObject = pObject;
		m_pFn = pFn;
		m_iEventType = UIEVENT__ALL;
		if(NULL != _sNotifyTypeName)
			m_sNotifyTypeName = _sNotifyTypeName;
	}

	CDelegateBase::CDelegateBase(const CDelegateBase& rhs) 
	{
		m_pObject = rhs.m_pObject;
		m_pFn = rhs.m_pFn; 
		m_iEventType = rhs.m_iEventType;
		m_sNotifyTypeName = rhs.m_sNotifyTypeName.GetData();
	}

	CDelegateBase::~CDelegateBase()
	{
		if(!m_sNotifyTypeName.IsEmpty())
			m_sNotifyTypeName.Empty();
	}

	bool CDelegateBase::Equals(const CDelegateBase& rhs) const 
	{
		return m_pObject == rhs.m_pObject && m_pFn == rhs.m_pFn && m_iEventType == rhs.m_iEventType && m_sNotifyTypeName == rhs.m_sNotifyTypeName.GetData(); 
	}

	void* CDelegateBase::GetFn() 
	{
		return m_pFn; 
	}

	void* CDelegateBase::GetObject() 
	{
		return m_pObject; 
	}

	CEventSource::~CEventSource()
	{
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBase* pObject = static_cast<CDelegateBase*>(m_aDelegates[i]);
			if( pObject) delete pObject;
		}
	}

	CEventSource::operator bool()
	{
		return m_aDelegates.GetSize() > 0;
	}

	void CEventSource::operator+= (const CDelegateBase& d)
	{ 
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBase* pObject = static_cast<CDelegateBase*>(m_aDelegates[i]);
			if( pObject && pObject->Equals(d) ) return;
		}

		m_aDelegates.Add(d.Copy());
	}

	void CEventSource::operator+= (const CDelegateBase* d)
	{
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBase* pObject = static_cast<CDelegateBase*>(m_aDelegates[i]);
			if( pObject && pObject->Equals(*d) ) return;
		}

		m_aDelegates.Add(d->Copy());
	}

	void CEventSource::operator-= (const CDelegateBase& d) 
	{
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBase* pObject = static_cast<CDelegateBase*>(m_aDelegates[i]);
			if( pObject && pObject->Equals(d) ) {
				delete pObject;
				m_aDelegates.Remove(i);
				return;
			}
		}
	}
		
	bool CEventSource::operator() (void* param) 
	{
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBase* pObject = static_cast<CDelegateBase*>(m_aDelegates[i]);
			if( pObject && !(*pObject)(param) ) return false;
		}
		return true;
	}

	bool CEventSource::operator() (TEventUI* pTEventUI,EVENTTYPE_UI _iEventType) 
	{
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBase* pObject = static_cast<CDelegateBase*>(m_aDelegates[i]);
			if( pObject && !(*pObject)(pTEventUI,_iEventType) ) return false;
		}
		return true;
	}

	bool CEventSource::operator() (TNotifyUI* pTNotifyUI,CDuiString& _sNotifyTypeName) 
	{
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBase* pObject = static_cast<CDelegateBase*>(m_aDelegates[i]);
			if( pObject && !(*pObject)(pTNotifyUI,_sNotifyTypeName.GetData()) ) return false;
		}
		return true;
	}

} // namespace UiLib
