#include <iostream>
#include <acapture/CDxDev.h>

int main(int argc, char* argv[])
{
	std::locale::global(std::locale("kor"));

	CoInitialize(NULL);

	{
		CDxDev dev;
		HRESULT hr = dev.devRead(CLSID_AudioInputDeviceCategory);
		if(FAILED(hr)){
			std::wcout<<L"Error: "<<dev.errorMsg()<<std::endl;
			exit(1);
		}
	}

	CoUninitialize();

	std::wcout<<L"end of main"<<std::endl;
	return 0;
}