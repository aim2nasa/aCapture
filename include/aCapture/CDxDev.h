#ifndef __CDXDEV_H__
#define __CDXDEV_H__

#include <aCapture/typedef.h>
#include <dshow.h>
#include <list>

class DLLEXP CName{
public:
	CName();
	CName(String friendly);
	virtual ~CName();

	String	m_friendly;
	String	m_full;
};

class DLLEXP CDxDev{
public:
	CDxDev();
	virtual ~CDxDev();

	void hrFailed(HRESULT hr);
	String errorMsg();

	HRESULT devRead(REFCLSID clsidDeviceClass);

	std::list<CName>& names();

protected:
	String				m_errorMsg;
	std::list<CName>	m_names;
	ICreateDevEnum*		m_pDeviceEnum;
};

#endif