#ifndef __CDXDEV_H__
#define __CDXDEV_H__

#include <aCapture/typedef.h>
#include <dshow.h>
#include <list>
#include <map>

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

	class CDev{
	public:
		CDev():m_pMonik(NULL),m_pFilter(NULL){}
		virtual ~CDev(){}

		IMoniker*		m_pMonik;
		IBaseFilter*	m_pFilter;
	};

protected:
	String					m_errorMsg;
	std::list<CName>		m_names;
	ICreateDevEnum*			m_pDeviceEnum;
	IGraphBuilder*			m_pGraph;
	IMediaControl*			m_pControl;
	std::map<String,CDev>	m_map;
};

#endif