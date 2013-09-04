#include <aCapture/CDxDev.h>
#include <atlbase.h>
#include <assert.h>

CDxDev::CDxDev()
:m_pNames(new std::list<CDxDev::CName>),m_pDeviceEnum(NULL)
{
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&m_pDeviceEnum);
	assert(SUCCEEDED(hr));
	assert(m_pDeviceEnum);
}

CDxDev::~CDxDev()
{
	m_pDeviceEnum->Release();
	delete m_pNames;
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

	m_pNames->clear();
	HRESULT hr = m_pDeviceEnum->CreateClassEnumerator(clsidDeviceClass, &pEnumCat, 0);// Enumerate the specified device, distinguished by DEVICE_CLSID
	if(hr == S_OK) {
		IMoniker *pDeviceMonik = NULL;
		ULONG cFetched;
		while(pEnumCat->Next(1,&pDeviceMonik,&cFetched)==S_OK){
			CComPtr<IPropertyBag> pPropBag;
			hr = pDeviceMonik->BindToStorage(0,0,IID_IPropertyBag,(void**)&pPropBag);
			if(SUCCEEDED(hr)){
				VariantInit(&varName);
				hr = pPropBag->Read(L"FriendlyName",&varName,0);
				if(SUCCEEDED(hr)) {
					m_pNames->push_back(CName(String(varName.bstrVal)));
				}else {
					hrFailed(hr);
				}
				VariantClear(&varName);
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