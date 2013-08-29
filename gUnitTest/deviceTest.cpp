#include <gtest/gtest.h>
#include <objbase.h>

void comInit()
{
	HRESULT hr = CoInitialize(NULL);
	EXPECT_EQ(hr,S_OK);
	hr = CoInitialize(NULL);
	EXPECT_EQ(hr,S_FALSE);
}

void comUninit()
{
	CoUninitialize();
}

TEST(comInit,deviceTest)
{
	comInit();
	comUninit();
}