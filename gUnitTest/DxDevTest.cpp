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
	EXPECT_GT(names.size(),1);	//����� �Է���ġ�� �ϳ� �̻��̶�� ����

	CoUninitialize();
}
