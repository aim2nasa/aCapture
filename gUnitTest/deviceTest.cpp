#include <gtest/gtest.h>
#include <objbase.h>
#include <atlbase.h>
#include <strmif.h>
#include <uuids.h>

TEST(comInit,deviceTest)
{
	HRESULT hr = CoInitialize(NULL);
	if(hr==S_OK) {
		hr = CoInitialize(NULL);
		EXPECT_EQ(hr,S_FALSE);
	}
	CoUninitialize();
}

TEST(devEnum,deviceTest)
{
	CoInitialize(NULL);

	CComPtr<ICreateDevEnum> pDeviceEnum;
	EXPECT_EQ(pDeviceEnum,reinterpret_cast<ICreateDevEnum*>(NULL));

	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,IID_ICreateDevEnum, (void **)&pDeviceEnum);
	if(FAILED(hr)) 
		FAIL();

	EXPECT_NE(pDeviceEnum,reinterpret_cast<ICreateDevEnum*>(NULL));

	CoUninitialize();
}

HRESULT deviceReader(REFCLSID clsidDeviceClass,ICreateDevEnum *pDeviceEnum)
{
	CComPtr <IEnumMoniker> pEnumCat;
	VARIANT varName;

	HRESULT hr = pDeviceEnum->CreateClassEnumerator(clsidDeviceClass, &pEnumCat, 0);// Enumerate the specified device, distinguished by DEVICE_CLSID
	if (hr == S_OK) {
		IMoniker *pDeviceMonik = NULL;
		ULONG cFetched;
		while(pEnumCat->Next(1,&pDeviceMonik,&cFetched)==S_OK){
			CComPtr<IPropertyBag> pPropBag;
			hr = pDeviceMonik->BindToStorage(0,0,IID_IPropertyBag,(void**)&pPropBag);
			if(SUCCEEDED(hr)){
				VariantInit(&varName);
				hr = pPropBag->Read(L"FriendlyName",&varName,0);
				if(SUCCEEDED(hr)) {
					std::cout<<varName.bstrVal<<std::endl;
				}
				VariantClear(&varName);
			}
			pDeviceMonik->Release();
		}
	}
	return hr;
}

TEST(devRead,deviceTest)
{
	CoInitialize(NULL);

	CComPtr<ICreateDevEnum> pDeviceEnum;
	EXPECT_EQ(pDeviceEnum,reinterpret_cast<ICreateDevEnum*>(NULL));

	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,IID_ICreateDevEnum, (void **)&pDeviceEnum);
	if(FAILED(hr)) 
		FAIL();

	hr = deviceReader(CLSID_AudioInputDeviceCategory,pDeviceEnum);
	if(FAILED(hr)) 
		FAIL();

	EXPECT_NE(pDeviceEnum,reinterpret_cast<ICreateDevEnum*>(NULL));
	CoUninitialize();
}