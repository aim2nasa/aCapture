#include <gtest/gtest.h>
#include <aCapture/CDxDev.h>

TEST(devRead,DxDevTest)
{
	CoInitialize(NULL);

	CDxDev dev;
	HRESULT hr = dev.devRead(CLSID_AudioInputDeviceCategory);
	if(FAILED(hr)) { FAIL(); }

	CoUninitialize();
}
