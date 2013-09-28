#include <aCapture/CDxDev.h>
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

HRESULT CDxDev::devRead(REFCLSID clsidDeviceClass)
{
	CComPtr <IEnumMoniker> pEnumCat;
	VARIANT varName;

	m_names.clear();
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

				m_names.push_back(name);
			}else {
				hrFailed(hr);
			}
			pDeviceMonik->Release();
		}
	}else{
		hrFailed(hr);
	}
	return hr;
}

std::list<CName>& CDxDev::names()
{
	return m_names;
}