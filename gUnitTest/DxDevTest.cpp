#include <locale>
#include <gtest/gtest.h>
#include <aCapture/CDxDev.h>
#include <iostream>

TEST(devRead,DxDevTest)
{
	CoInitialize(NULL);

	CDxDev dev;
	bool b = dev.devRead(CLSID_AudioInputDeviceCategory);
	EXPECT_EQ(b,true);

	//�ϳ� �̻��� ������ �ݵ�� ���� �͵��� �̸��� ���;� �Ѵ�
	NameList* pNames = dev.names(CLSID_AudioInputDeviceCategory);
	EXPECT_NE(pNames,reinterpret_cast<NameList*>(NULL));
	for(NameList::iterator it=pNames->begin();it!=pNames->end();it++) {	
		EXPECT_GT((*it).m_friendly.size(),0);
		EXPECT_GT((*it).m_full.size(),0);
	}

	CoUninitialize();
}
