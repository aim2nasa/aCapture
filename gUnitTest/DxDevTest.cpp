#include <locale>
#include <gtest/gtest.h>
#include <aCapture/CDxDev.h>
#include <iostream>

TEST(devRead,DxDevTest)
{
	CoInitialize(NULL);

	CDxDev dev;
	HRESULT hr = dev.devRead(CLSID_AudioInputDeviceCategory);
	if(FAILED(hr)) { FAIL(); }

	std::list<CName> names = dev.names();
	EXPECT_GT(names.size(),1);	//오디오 입력장치가 하나 이상이라고 가정

	CoUninitialize();
}
