#include <iostream>
#include <acapture/CDxDev.h>

int main(int argc, char* argv[])
{
	std::locale::global(std::locale("kor"));

	CoInitialize(NULL);

	CDxDev* pDev = new CDxDev();
	HRESULT hr = pDev->devRead(CLSID_AudioInputDeviceCategory);
	if(FAILED(hr)){
		std::wcout<<L"Error: "<<pDev->errorMsg()<<std::endl;
		exit(1);
	}

	std::list<CName> names = pDev->names();
	std::wcout<<"Devices :"<<names.size()<<std::endl;

	for(std::list<CName>::iterator it=names.begin();it!=names.end();it++){
		std::wcout<<"\tname:"<<(*it).m_friendly<<std::endl;
	}

	delete pDev;

	CoUninitialize();

	std::wcout<<L"end of main"<<std::endl;
	return 0;
}