#include <iostream>
#include <aCapture/CDxDev.h>
#include <conio.h>
#include <assert.h>

using namespace std;

int main (void)
{
	setlocale(LC_ALL,"Korean");

	HRESULT hr = CoInitialize(NULL);// Initialise COM
	assert(SUCCEEDED(hr));

	CDxDev* pDxDev = new CDxDev();

	//Front mic input
	GUID DEVICE_CLSID = CLSID_AudioInputDeviceCategory;	//the input device category
	String deviceName1(_T("마이크(Realtek High Definition Aud"));

	if(pDxDev->add(DEVICE_CLSID,deviceName1)) {
		wcout<<"<"<<deviceName1<<"> added"<<endl;
	}else{
		wcout<<"<"<<deviceName1<<"> add failed :"<<pDxDev->errorMsg()<<endl;
		return 1;
	}

	//Default output device
	DEVICE_CLSID = CLSID_AudioRendererCategory;	//the audio renderer device category
	String deviceName2(_T("스피커(Realtek High Definition Aud"));

	if(pDxDev->add(DEVICE_CLSID,deviceName2)) {
		wcout<<"<"<<deviceName2<<"> added"<<endl;
	}else{
		wcout<<"<"<<deviceName2<<"> add failed :"<<pDxDev->errorMsg()<<endl;
		return 1;
	}

	//Connect input to output
	if(pDxDev->connect(deviceName1,String(_T("Capture")),deviceName2,String(_T("Audio Input pin (rendered)")))){
		wcout<<"Two filters are connected"<<endl;
	}else{
		wcout<<"failed to connect filters : "<<pDxDev->errorMsg()<<endl;
		return 1;
	}

	//Now run the graph
	if(pDxDev->run()) {
		wcout<<_T("now running")<<endl;
	}else{
		wcout<<_T("run failed,")<<pDxDev->errorMsg()<<endl;
		return 1;
	}

	wcout<<"Close the window to exit or hit any key"<<endl;
	while(!_kbhit())
	{
	}
	delete pDxDev;

	CoUninitialize();
}