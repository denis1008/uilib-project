#ifndef __UIDELEGATE_H__
#define __UIDELEGATE_H__

#pragma once

namespace UiLib {
	enum EVENTTYPE_UI;
	typedef struct tagTEventUI TEventUI;
	typedef struct tagTNotifyUI TNotifyUI;
	
	class UILIB_API CDelegateBase	 
	{
	public:
		CDelegateBase(void* pObject, void* pFn);
		CDelegateBase(void* pObject, void* pFn,EVENTTYPE_UI _iEventType);
		CDelegateBase(void* pObject, void* pFn,LPCTSTR _sNotifyTypeName);
		CDelegateBase(const CDelegateBase& rhs);

		virtual ~CDelegateBase();
		void* GetFn();
		bool Equals(const CDelegateBase& rhs) const;
		bool operator() (void* param){return Invoke(param);};
		bool operator() (TEventUI* pTEventUI,EVENTTYPE_UI _iEventType){return Invoke(pTEventUI);};
		bool operator() (TNotifyUI* pTNotifyUI,LPCTSTR _sNotifyTypeName){return Invoke(pTNotifyUI);};
		virtual CDelegateBase* Copy() const = 0; // add const for gcc

	protected:
		void* GetObject();
		void SetEventType(EVENTTYPE_UI _iEventType){m_iEventType = _iEventType;};
		void SetNotifyTypeName(CDuiString& _sNotifyTypeName){m_sNotifyTypeName = _sNotifyTypeName.GetData();};
		EVENTTYPE_UI GetEventType(){return m_iEventType;};
		CDuiString GetNotifyTypeName(){return m_sNotifyTypeName.GetData();};
		virtual bool Invoke(void* param) = 0;
		virtual bool Invoke(TEventUI* pTEventUI) = 0;
		virtual bool Invoke(TNotifyUI* pTNotifyUI) = 0;

	private:
		void* m_pObject;
		void* m_pFn;
	protected:
		EVENTTYPE_UI m_iEventType;
		CDuiString m_sNotifyTypeName;
	};

	class UILIB_API CDelegateStatic: public CDelegateBase
	{
		typedef bool (*Fn)(void*);
		typedef bool (*TEventFun)(TEventUI* pTEventUI);
		typedef bool (*TNotifyFun)(TNotifyUI* pTNotifyUI);
	public:
		CDelegateStatic(Fn pFn) : CDelegateBase(NULL, pFn) { } 
		CDelegateStatic(TEventFun pFn,EVENTTYPE_UI _iEventType = (EVENTTYPE_UI)0) : CDelegateBase(NULL, pFn,_iEventType) { } 
		CDelegateStatic(TNotifyFun pFn,LPCTSTR _sNotifyTypeName = NULL) : CDelegateBase(NULL, pFn,_sNotifyTypeName) { } 
		CDelegateStatic(const CDelegateStatic& rhs) : CDelegateBase(rhs) { } 
		virtual CDelegateBase* Copy() const { return new CDelegateStatic(*this); }

	protected:
		virtual bool Invoke(void* param)
		{
			Fn pFn = (Fn)GetFn();
			return (*pFn)(param); 
		}

		virtual bool Invoke(TEventUI* pTEventUI)
		{
			TEventFun pFn = (TEventFun)GetFn();
			return !(*pFn)(pTEventUI); 
		};

		virtual bool Invoke(TNotifyUI* pTNotifyUI)
		{
			TNotifyFun pFn = (TNotifyFun)GetFn();
			return (*pFn)(pTNotifyUI); 
		};
	};

	template <class O, class T>
	class UILIB_API CDelegate : public CDelegateBase
	{
		typedef bool (T::* Fn)(void*);
		typedef bool (T::*TEventFun)(TEventUI* pTEventUI);
		typedef bool (T::*TNotifyFun)(TNotifyUI* pTNotifyUI);
	public:
		CDelegate(O* pObj, Fn pFn) : CDelegateBase(pObj, &pFn), m_pFn(pFn),m_pTEventFun(NULL),m_pTNotifyFun(NULL){}
		CDelegate(O* pObj, TEventFun pFn,EVENTTYPE_UI _iEventType = (EVENTTYPE_UI)0) : CDelegateBase(pObj, &pFn,_iEventType), m_pFn(NULL),m_pTEventFun(pFn),m_pTNotifyFun(NULL) { }
		CDelegate(O* pObj, TNotifyFun pFn,LPCTSTR _sNotifyTypeName = NULL) : CDelegateBase(pObj, &pFn,_sNotifyTypeName), m_pFn(NULL),m_pTEventFun(NULL),m_pTNotifyFun(pFn) { }
		CDelegate(const CDelegate& rhs) : CDelegateBase(rhs) { m_pFn = rhs.m_pFn;m_pTEventFun = rhs.m_pTEventFun;m_pTNotifyFun = rhs.m_pTNotifyFun;} 
		virtual CDelegateBase* Copy() const { return new CDelegate(*this); }

	protected:
		virtual bool Invoke(void* param)
		{
			O* pObject = (O*) GetObject();
			if(m_pFn)
				return (pObject->*m_pFn)(param);
			else if(m_pTEventFun)
				return Invoke((TEventUI*)param);
			else if(m_pTNotifyFun)
				return Invoke((TNotifyUI*)param);

			return true;
		} 

		virtual bool Invoke(TEventUI* pTEventUI)
		{
			O* pObject = (O*) GetObject();
			if(pTEventUI && GetEventType() == (EVENTTYPE_UI)0)
				return (pObject->*m_pTEventFun)(pTEventUI); 
			else if(pTEventUI && pTEventUI->Type == GetEventType())
				return (pObject->*m_pTEventFun)(pTEventUI);

			return true;
		};

		virtual bool Invoke(TNotifyUI* pTNotifyUI)
		{
			O* pObject = (O*) GetObject();
			if(GetNotifyTypeName().IsEmpty())
				return (pObject->*m_pTNotifyFun)(pTNotifyUI);
			else if(pTNotifyUI && pTNotifyUI->sType == GetNotifyTypeName())
				return (pObject->*m_pTNotifyFun)(pTNotifyUI);

			return true;
		};

	private:
		Fn m_pFn;
		TEventFun m_pTEventFun;
		TNotifyFun m_pTNotifyFun;
	};

	template <class O, class T>
	CDelegate<O, T> MakeDelegate(O* pObject, bool (T::* pFn)(void*))
	{
		return CDelegate<O, T>(pObject, pFn);
	}

	template <class O, class T>
	CDelegate<O, T> MakeDelegate(O* pObject, bool (T::* pFn)(TEventUI*),EVENTTYPE_UI _iEventType)
	{
		return CDelegate<O, T>(pObject, pFn,_iEventType);
	}

	template <class O, class T>
	CDelegate<O, T>* MakeDelegate(O* pObject, bool (T::* pFn)(TNotifyUI*),LPCTSTR _sNotifyTypeName)
	{
		return new CDelegate<O, T>(pObject, pFn,_sNotifyTypeName);
	}

	inline CDelegateStatic MakeDelegate(bool (*pFn)(void*))
	{
		return CDelegateStatic(pFn); 
	}

	inline CDelegateStatic MakeDelegate(bool (*pFn)(TEventUI*),EVENTTYPE_UI _iEventType)
	{
		return CDelegateStatic(pFn,_iEventType); 
	}

	inline CDelegateStatic MakeDelegate(bool (*pFn)(TNotifyUI*),LPCTSTR _sNotifyTypeName)
	{
		return CDelegateStatic(pFn,_sNotifyTypeName); 
	}


	typedef bool (*FnType)(void*);
	typedef bool (*TEventFunType)(TEventUI* pTEventUI);
	typedef bool (*TNotifyFunType)(TNotifyUI* pTNotifyUI);

	class UILIB_API CEventSource
	{
	public:
		~CEventSource();
		operator bool();
		void operator+= (const CDelegateBase& d); // add const for gcc
		void operator+= (const CDelegateBase* d); // add const for gcc
		void operator+= (FnType pFn){(*this) += MakeDelegate(pFn);};
		void operator+= (CDelegateStatic* _Delegate){(*this) += *_Delegate;};
		void operator-= (const CDelegateBase& d);
		void operator-= (FnType pFn){(*this) -= MakeDelegate(pFn);};
		void operator-= (CDelegateStatic* _Delegate){(*this) -= *_Delegate;};
		bool operator() (void* param);
		bool operator() (TEventUI* pTEventUI,EVENTTYPE_UI _iEventType);
		bool operator() (TNotifyUI* pTNotifyUI,CDuiString& _sNotifyTypeName);

	protected:
		CStdPtrArray m_aDelegates;
	};

} // namespace UiLib

#endif // __UIDELEGATE_H__