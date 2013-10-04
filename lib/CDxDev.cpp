#include <aCapture/CDxDev.h>
#include <aCapture/CDxHelper.h>
#include <atlbase.h>
#include <assert.h>

CName::CName()
{

}

CName::CName(String friendly)
:m_friendly(friendly)
{

}

CName::~CName()
{

}

CDxDev::CDxDev()
:m_pDeviceEnum(NULL),m_pGraph(NULL),m_pControl(NULL)
{
	HRESULT hr;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&m_pDeviceEnum);
	assert(SUCCEEDED(hr));
	assert(m_pDeviceEnum);

	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,IID_IGraphBuilder, (void**)&m_pGraph);
	assert(SUCCEEDED(hr));
	assert(m_pGraph);

	hr = m_pGraph->QueryInterface(IID_IMediaControl,(void**)&m_pControl);
	assert(SUCCEEDED(hr));
	assert(m_pControl);
}

CDxDev::~CDxDev()
{
	m_pControl->Release();
	m_pGraph->Release();
	m_pDeviceEnum->Release();
}

String CDxDev::errorMsg()
{
	return m_errorMsg;
}

void CDxDev::hrFailed(HRESULT hr)
{
	TCHAR szErr[MAX_ERROR_TEXT_LEN];
	DWORD res = AMGetErrorText(hr,szErr,MAX_ERROR_TEXT_LEN);
	if(res==0){
		StringCchPrintf(szErr,MAX_ERROR_TEXT_LEN,L"Unknown Error: 0x%2x",hr);
		m_errorMsg = String(szErr);
	}
}

bool CDxDev::devRead(REFCLSID clsidDeviceClass)
{
	NameList names;
	CComPtr <IEnumMoniker> pEnumCat;
	VARIANT varName;

	HRESULT hr = m_pDeviceEnum->CreateClassEnumerator(clsidDeviceClass, &pEnumCat, 0);// Enumerate the specified device, distinguished by DEVICE_CLSID
	if(hr == S_OK) {
		IMoniker *pDeviceMonik = NULL;
		ULONG cFetched;
		while(pEnumCat->Next(1,&pDeviceMonik,&cFetched)==S_OK){
			CComPtr<IPropertyBag> pPropBag;
			hr = pDeviceMonik->BindToStorage(0,0,IID_IPropertyBag,(void**)&pPropBag);
			if(SUCCEEDED(hr)){
				CName name;

				LPOLESTR pDisplayName=NULL;
				if(SUCCEEDED(pDeviceMonik->GetDisplayName(0,0,&pDisplayName))){
					name.m_full = String(pDisplayName);
					CoTaskMemFree(pDisplayName);
				}

				VariantInit(&varName);
				hr = pPropBag->Read(L"FriendlyName",&varName,0);
				if(SUCCEEDED(hr)) {
					name.m_friendly = String(varName.bstrVal);
				}else {
					hrFailed(hr);
				}
				VariantClear(&varName);

				names.push_back(name);
			}else {
				hrFailed(hr);
			}
			pDeviceMonik->Release();
		}
	}else{
		hrFailed(hr);
	}

	std::pair<std::map<String,NameList>::iterator,bool> ret;
	ret = m_clsidMap.insert(std::pair<String,NameList>(strFromGuid(clsidDeviceClass),names));
	if(!ret.second) m_errorMsg = String(_T("insert to map failed "))+strFromGuid(clsidDeviceClass);
	return ret.second;
}

NameList* CDxDev::names(REFCLSID clsidDeviceClass)
{
	std::map<String,NameList>::iterator it = m_clsidMap.find(strFromGuid(clsidDeviceClass));
	if(it!=m_clsidMap.end()) return &it->second;
	return NULL;
}

bool CDxDev::add(GUID devClsid,String devName)
{
	IMoniker *pMonik = NULL;
	HRESULT hr = CDxHelper::read(m_pDeviceEnum,devClsid,devName,&pMonik);
	if(FAILED(hr)) {
		hrFailed(hr);
		return false;
	}
	
	IBaseFilter *pFilter = NULL;
	hr = CDxHelper::bind(pMonik,&pFilter);
	if(FAILED(hr)) {
		hrFailed(hr);
		return false;
	}

	hr = CDxHelper::addToGraph(m_pGraph,pFilter,devName);
	if(FAILED(hr)){
		hrFailed(hr);
		return false;
	}

	std::pair<std::map<String,CFilter>::iterator,bool> ret;
	ret = m_map.insert(std::pair<String,CFilter>(devName,CFilter(pMonik,pFilter)));
	if(!ret.second) m_errorMsg = String(_T("insert to map failed "))+devName;
	return ret.second;
}

CDxDev::CFilter* CDxDev::get(String devName)
{
	std::map<String,CFilter>::iterator it = m_map.find(devName);
	if(it!=m_map.end()) return &it->second;
	return NULL;
}

bool CDxDev::connect(String devName1,String dev1Pin,String devName2,String dev2Pin)
{
	CDxDev::CFilter *pFilter1 = get(devName1);
	if(!pFilter1) {
		m_errorMsg = String(_T("could not find filter:"))+devName1;
		return false;
	}

	CDxDev::CFilter *pFilter2 = get(devName2);
	if(!pFilter2) {
		m_errorMsg = String(_T("could not find filter:"))+devName2;
		return false;
	}

	HRESULT hr = CDxHelper::conFilter(pFilter1->m_pFilter,dev1Pin,pFilter2->m_pFilter,dev2Pin);
	if(FAILED(hr)) {
		hrFailed(hr);
		m_errorMsg += String(_T(",conFilter failed between pin1="))+dev1Pin+String(_T(" of "))+devName1;
		m_errorMsg += String(_T(",pin2="))+dev2Pin+String(_T(" of "))+devName2;
		return false;
	}
	return true;
}

bool CDxDev::run()
{
	HRESULT hr = CDxHelper::run(m_pControl);
	if(FAILED(hr)) {
		hrFailed(hr);
		return false;
	}
	return true;
}

String CDxDev::strFromGuid(REFCLSID clsidDeviceClass)
{
	wchar_t clsid[128];
	StringFromGUID2(clsidDeviceClass,clsid,sizeof(clsid)/sizeof(wchar_t));
	return String(clsid);
}