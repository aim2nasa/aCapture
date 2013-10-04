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

	//하나 이상이 나오면 반드시 오든 것들은 이름이 나와야 한다
	NameList* pNames = dev.names(CLSID_AudioInputDeviceCategory);
	EXPECT_NE(pNames,reinterpret_cast<NameList*>(NULL));
	for(NameList::iterator it=pNames->begin();it!=pNames->end();it++) {	
		EXPECT_GT((*it).m_friendly.size(),0);
		EXPECT_GT((*it).m_full.size(),0);
	}

	CoUninitialize();
}
