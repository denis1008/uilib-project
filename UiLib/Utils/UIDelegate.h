#ifndef __UIDELEGATE_H__
#define __UIDELEGATE_H__

#pragma once

namespace UiLib {
	enum EVENTTYPE_UI;
	typedef struct tagTEventUI TEventUI;
	typedef struct tagTNotifyUI TNotifyUI;

	class CDelegateBase;
	
	class UILIB_API IDelegate
	{
	public:
		virtual void* GetFn() const = 0;
		virtual LPARAM GetLParam() const = 0;
		virtual WPARAM GetWParam() const = 0;
	public:
		virtual CDelegateBase* Copy() const = 0;
		virtual bool Invoke(void* pParam,LPARAM lParam = NULL,WPARAM wParam = NULL) = 0;
		virtual bool Invoke(TEventUI* pTEventUI,LPARAM lParam = NULL,WPARAM wParam = NULL) = 0;
		virtual bool Invoke(TNotifyUI* pTNotifyUI,LPARAM lParam = NULL,WPARAM wParam = NULL) = 0;
	};

	class UILIB_API CDelegateBase : public IDelegate
	{
	public:
		CDelegateBase(void* pObject, void* pFn,LPARAM lParam = NULL,WPARAM wParam = NULL);
		CDelegateBase(void* pObject, void* pFn,EVENTTYPE_UI _iEventType,LPARAM lParam = NULL,WPARAM wParam = NULL);
		CDelegateBase(void* pObject, void* pFn,LPCTSTR _sNotifyTypeName,LPARAM lParam = NULL,WPARAM wParam = NULL);
		CDelegateBase(const CDelegateBase& rhs);

		virtual ~CDelegateBase();
		void* GetFn() const { return m_pFn;};
		LPARAM GetLParam() const { return m_lParam;};
		WPARAM GetWParam() const { return m_wParam;};
		bool Equals(const CDelegateBase& rhs) const;
		bool operator() (void* param,LPARAM lParam = NULL,WPARAM wParam = NULL){return Invoke(param,lParam,wParam);};
		bool operator() (TEventUI* pTEventUI,EVENTTYPE_UI _iEventType,LPARAM lParam = NULL,WPARAM wParam = NULL){return Invoke(pTEventUI,lParam,wParam);};
		bool operator() (TNotifyUI* pTNotifyUI,LPCTSTR _sNotifyTypeName,LPARAM lParam = NULL,WPARAM wParam = NULL){return Invoke(pTNotifyUI,lParam,wParam);};

		void* GetObj() const {return m_pObject;};
	protected:
		void SetEventType(EVENTTYPE_UI _iEventType){m_iEventType = _iEventType;};
		void SetNotifyTypeName(CDuiString& _sNotifyTypeName){m_sNotifyTypeName = _sNotifyTypeName.GetData();};
		EVENTTYPE_UI GetEventType(){return m_iEventType;};
		CDuiString GetNotifyTypeName(){return m_sNotifyTypeName.GetData();};

	private:
		void* m_pObject;
		void* m_pFn;
		LPARAM m_lParam;
		WPARAM m_wParam;
	protected:
		EVENTTYPE_UI m_iEventType;
		CDuiString m_sNotifyTypeName;
	};

	class UILIB_API CDelegateStatic: public CDelegateBase
	{
		typedef bool (*Fn)(void* pParam,LPARAM lParam,WPARAM wParam);
		typedef bool (*TEventFun)(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
		typedef bool (*TNotifyFun)(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);
	public:
		CDelegateStatic(Fn pFn,LPARAM lParam = NULL,WPARAM wParam = NULL) : CDelegateBase(NULL, pFn,lParam,wParam) { } 
		CDelegateStatic(TEventFun pFn,EVENTTYPE_UI _iEventType = (EVENTTYPE_UI)0,LPARAM lParam = NULL,WPARAM wParam = NULL) : CDelegateBase(NULL, pFn,_iEventType,lParam,wParam) { } 
		CDelegateStatic(TNotifyFun pFn,LPCTSTR _sNotifyTypeName = NULL,LPARAM lParam = NULL,WPARAM wParam = NULL) : CDelegateBase(NULL, pFn,_sNotifyTypeName,lParam,wParam) { } 
		CDelegateStatic(const CDelegateStatic& rhs) : CDelegateBase(rhs) { } 
		virtual CDelegateBase* Copy() const { return new CDelegateStatic(*this); }

	protected:
		virtual bool Invoke(void* param,LPARAM lParam = NULL,WPARAM wParam = NULL)
		{
			Fn pFn = (Fn)GetFn();
			return (*pFn)(param,lParam,wParam); 
		}

		virtual bool Invoke(TEventUI* pTEventUI,LPARAM lParam = NULL,WPARAM wParam = NULL)
		{
			TEventFun pFn = (TEventFun)GetFn();
			return !(*pFn)(pTEventUI,lParam,wParam); 
		};

		virtual bool Invoke(TNotifyUI* pTNotifyUI,LPARAM lParam = NULL,WPARAM wParam = NULL)
		{
			TNotifyFun pFn = (TNotifyFun)GetFn();
			return (*pFn)(pTNotifyUI,lParam,wParam); 
		};
	};

	template <class O, class T,class P = LPARAM>
	class UILIB_API CDelegate : public CDelegateBase
	{
		typedef bool (T::* Fn)(void* pParam,P lParam,WPARAM wParam);
		typedef bool (T::*TEventFun)(TEventUI* pTEventUI,P lParam,WPARAM wParam);
		typedef bool (T::*TNotifyFun)(TNotifyUI* pTNotifyUI,P lParam,WPARAM wParam);
	public:
		CDelegate(O* pObj, Fn pFn,P lParam = NULL,WPARAM wParam = NULL) : CDelegateBase(pObj, *(void**)&pFn,(LPARAM)lParam,wParam), m_pFn(pFn),m_pTEventFun(NULL),m_pTNotifyFun(NULL){}
		CDelegate(O* pObj, TEventFun pFn,EVENTTYPE_UI _iEventType = (EVENTTYPE_UI)0,P lParam = NULL,WPARAM wParam = NULL) : CDelegateBase(pObj, *(void**)&pFn,_iEventType,(LPARAM)lParam,wParam), m_pFn(NULL),m_pTEventFun(pFn),m_pTNotifyFun(NULL) { }
		CDelegate(O* pObj, TNotifyFun pFn,LPCTSTR _sNotifyTypeName = NULL,P lParam = NULL,WPARAM wParam = NULL) : CDelegateBase(pObj, *(void**)&pFn,_sNotifyTypeName,(LPARAM)lParam,wParam), m_pFn(NULL),m_pTEventFun(NULL),m_pTNotifyFun(pFn) { }
		CDelegate(const CDelegate& rhs) : CDelegateBase(rhs) { m_pFn = rhs.m_pFn;m_pTEventFun = rhs.m_pTEventFun;m_pTNotifyFun = rhs.m_pTNotifyFun;} 
		virtual CDelegateBase* Copy() const { return new CDelegate(*this); }

	protected:
		virtual bool Invoke(void* param,LPARAM lParam = NULL,WPARAM wParam = NULL)
		{
			O* pObject = (O*) GetObj();
			if(pObject && m_pFn)
				return (pObject->*m_pFn)(param,(P)GetLParam(),GetWParam());
			else if(pObject && m_pTEventFun)
				return Invoke((TEventUI*)param,GetLParam(),GetWParam());
			else if(pObject && m_pTNotifyFun)
				return Invoke((TNotifyUI*)param,GetLParam(),GetWParam());

			return true;
		} 

		virtual bool Invoke(TEventUI* pTEventUI,LPARAM lParam = NULL,WPARAM wParam = NULL)
		{
			O* pObject = (O*) GetObj();
			if(pObject && pTEventUI && GetEventType() == (EVENTTYPE_UI)0)
				return (pObject->*m_pTEventFun)(pTEventUI,(P)GetLParam(),GetWParam()); 
			else if(pObject && pTEventUI && pTEventUI->Type == GetEventType())
				return (pObject->*m_pTEventFun)(pTEventUI,(P)GetLParam(),GetWParam());

			return true;
		};

		virtual bool Invoke(TNotifyUI* pTNotifyUI,LPARAM lParam = NULL,WPARAM wParam = NULL)
		{
			O* pObject = (O*) GetObj();
			if(pObject && GetNotifyTypeName().IsEmpty())
				return (pObject->*m_pTNotifyFun)(pTNotifyUI,(P)GetLParam(),GetWParam());
			else if(pObject && pTNotifyUI && pTNotifyUI->sType == GetNotifyTypeName())
				return (pObject->*m_pTNotifyFun)(pTNotifyUI,(P)GetLParam(),GetWParam());

			return true;
		};

	private:
		Fn m_pFn;
		TEventFun m_pTEventFun;
		TNotifyFun m_pTNotifyFun;
	};

	template <class O, class T,class P>
	CDelegate<O, T, P> MakeDelegate(O* pObject, bool (T::* pFn)(void* pParam,LPARAM lParam,WPARAM wParam),P lParam = NULL,WPARAM wParam = NULL)
	{
		return CDelegate<O, T, P>(pObject, pFn,lParam,wParam);
	}

	template <class O, class T,class P>
	CDelegate<O, T, P> MakeDelegate(O* pObject, bool (T::* pFn)(TEventUI*,P lParam,WPARAM wParam),EVENTTYPE_UI _iEventType,P lParam = NULL,WPARAM wParam = NULL)
	{
		return CDelegate<O, T, P>(pObject, pFn,_iEventType,lParam,wParam);
	}

	template <class O, class T,class P>
	CDelegate<O, T, P> MakeDelegate(O* pObject, bool (T::* pFn)(TNotifyUI*,P lParam,WPARAM wParam),LPCTSTR _sNotifyTypeName,P lParam = NULL,WPARAM wParam = NULL)
	{
		return CDelegate<O, T, P>(pObject, pFn,(LPCTSTR)_sNotifyTypeName,lParam,wParam);
	}

	inline CDelegateStatic MakeDelegate(bool (*pFn)(void* pParam,LPARAM lParam,WPARAM wParam),LPARAM lParam = NULL,WPARAM wParam = NULL)
	{
		return CDelegateStatic(pFn,lParam,wParam); 
	}

	inline CDelegateStatic MakeDelegate(bool (*pFn)(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam),EVENTTYPE_UI _iEventType,LPARAM lParam = NULL,WPARAM wParam = NULL)
	{
		return CDelegateStatic(pFn,_iEventType,lParam,wParam); 
	}

	inline CDelegateStatic MakeDelegate(bool (*pFn)(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam),LPCTSTR _sNotifyTypeName,LPARAM lParam = NULL,WPARAM wParam = NULL)
	{
		return CDelegateStatic(pFn,_sNotifyTypeName,lParam,wParam); 
	}

	typedef bool (*FnType)(void* pParam,LPARAM lParam,WPARAM wParam);
	typedef bool (*TEventFunType)(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
	typedef bool (*TNotifyFunType)(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);

	class UILIB_API CEventSource
	{
	public:
		~CEventSource();
		operator bool();
		void operator+= (const CDelegateBase& d); // add const for gcc
		void operator+= (FnType pFn){(*this) += MakeDelegate(pFn);};
		void operator-= (const CDelegateBase& d);
		void operator-= (FnType pFn){(*this) -= MakeDelegate(pFn);};
		bool operator() (void* param);
		bool operator() (TEventUI* pTEventUI);
		bool operator() (TNotifyUI* pTNotifyUI);

	protected:
		CStdPtrArray m_aDelegates;
	};

} // namespace UiLib

#endif // __UIDELEGATE_H__