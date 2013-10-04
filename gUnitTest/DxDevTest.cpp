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

	//�ϳ� �̻��� ������ �ݵ�� ���� �͵��� �̸��� ���;� �Ѵ�
	NameList names = dev.names();
	for(NameList::iterator it=names.begin();it!=names.end();it++) {	
		EXPECT_GT((*it).m_friendly.size(),0);
		EXPECT_GT((*it).m_full.size(),0);
	}

	CoUninitialize();
}
