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

typedef std::list<CName> NameList;

class DLLEXP CDxDev{
public:
	class CFilter{
	public:
		CFilter():m_pMonik(NULL),m_pFilter(NULL){}
		CFilter(IMoniker *pMon):m_pMonik(pMon),m_pFilter(NULL){}
		CFilter(IMoniker *pMon,IBaseFilter *pFilter):m_pMonik(pMon),m_pFilter(pFilter){}
		virtual ~CFilter(){}

		IMoniker*		m_pMonik;
		IBaseFilter*	m_pFilter;
	};

	CDxDev();
	virtual ~CDxDev();

	void hrFailed(HRESULT hr);
	String errorMsg();

	bool devRead(REFCLSID clsidDeviceClass);
	NameList* names(REFCLSID clsidDeviceClass);

	bool add(GUID devClsid,String devName);
	CFilter* get(String devName);
	bool connect(String dev1,String dev1Pin,String dev2,String dev2Pin);
	bool run();

	static String strFromGuid(REFCLSID clsidDeviceClass);

protected:
	String						m_errorMsg;
	ICreateDevEnum*				m_pDeviceEnum;
	IGraphBuilder*				m_pGraph;
	IMediaControl*				m_pControl;
	std::map<String,CFilter>	m_map;
	std::map<String,NameList>	m_clsidMap;
};

#endif