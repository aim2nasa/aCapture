#include <iostream>
#include <acapture/CDxDev.h>

void read(CDxDev* pDev,String devName,REFCLSID clsidDeviceClass)
{
	bool b = pDev->devRead(clsidDeviceClass);
	if(!b){
		std::wcout<<L"Error: "<<pDev->errorMsg()<<std::endl;
		return;
	}

	std::list<CName> names = pDev->names();
	std::wcout<<L"CLSID:"<<devName<<" ,Devices :"<<names.size()<<std::endl;

	for(std::list<CName>::iterator it=names.begin();it!=names.end();it++){
		std::wcout<<"\tname:"<<(*it).m_friendly<<std::endl;
	}
}

int main(int argc, char* argv[])
{
	std::locale::global(std::locale("kor"));

	CoInitialize(NULL);

	CDxDev* pDev = new CDxDev();

	read(pDev,L"CLSID_AudioInputDeviceCategory",CLSID_AudioInputDeviceCategory);
	read(pDev,L"CLSID_AudioCompressorCategory",CLSID_AudioCompressorCategory);
	read(pDev,L"CLSID_AudioRendererCategory",CLSID_AudioRendererCategory);
	read(pDev,L"CLSID_DeviceControlCategory",CLSID_DeviceControlCategory);
	read(pDev,L"CLSID_LegacyAmFilterCategory",CLSID_LegacyAmFilterCategory);
	read(pDev,L"CLSID_TransmitCategory",CLSID_TransmitCategory);
	read(pDev,L"CLSID_MidiRendererCategory",CLSID_MidiRendererCategory);
	read(pDev,L"CLSID_VideoInputDeviceCategory",CLSID_VideoInputDeviceCategory);
	read(pDev,L"CLSID_VideoCompressorCategory",CLSID_VideoCompressorCategory);
	read(pDev,L"CLSID_DVDHWDecodersCategory",CLSID_DVDHWDecodersCategory);

	delete pDev;

	CoUninitialize();

	std::wcout<<L"end of main"<<std::endl;
	return 0;
}