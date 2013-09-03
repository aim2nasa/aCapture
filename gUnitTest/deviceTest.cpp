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