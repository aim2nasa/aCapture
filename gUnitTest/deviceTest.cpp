#include <gtest/gtest.h>
#include <objbase.h>

TEST(comInit,deviceTest)
{
	HRESULT hr = CoInitialize(NULL);
	EXPECT_EQ(hr,S_OK);
	CoUninitialize();
}