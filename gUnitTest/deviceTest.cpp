#include <gtest/gtest.h>
#include <objbase.h>

TEST(comInit,deviceTest)
{
	HRESULT hr = CoInitialize(NULL);
	EXPECT_EQ(hr,S_OK);
	hr = CoInitialize(NULL);
	EXPECT_EQ(hr,S_FALSE);
	CoUninitialize();
}