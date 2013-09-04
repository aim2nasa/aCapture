#ifndef __CDXDEV_H__
#define __CDXDEV_H__

#include <aCapture/typedef.h>
#include <dshow.h>
#include <list>

class DLLEXP CDxDev{
public:
	CDxDev();
	virtual ~CDxDev();

	void hrFailed(HRESULT hr);
	String errorMsg();

	HRESULT devRead(REFCLSID clsidDeviceClass);

	class CName{
	public:
		CName(String friendly):m_friendly(friendly){}
		CName(String full,String friendly):m_full(full),m_friendly(friendly){}
		String m_full;
		String m_friendly;
	};

protected:
	String				m_errorMsg;
	std::list<CName>*	m_pNames;
	ICreateDevEnum*		m_pDeviceEnum;
};

#endif