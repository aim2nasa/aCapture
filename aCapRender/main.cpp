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
	String deviceName1(_T("마이크(Realtek High Definition Aud"));

	if(pDxDev->add(CLSID_AudioInputDeviceCategory,deviceName1)) {
		wcout<<"Filter <"<<deviceName1<<"> added"<<endl;
	}else{
		wcout<<"<"<<deviceName1<<"> add failed :"<<pDxDev->errorMsg()<<endl;
		return 1;
	}

	//Default output device
	String deviceName2(_T("스피커(Realtek High Definition Aud"));

	if(pDxDev->add(CLSID_AudioRendererCategory,deviceName2)) {
		wcout<<"Filter <"<<deviceName2<<"> added"<<endl;
	}else{
		wcout<<"<"<<deviceName2<<"> add failed :"<<pDxDev->errorMsg()<<endl;
		return 1;
	}

	//Connect input to output
	String pin1(_T("Capture")),pin2(_T("Audio Input pin (rendered)"));
	if(pDxDev->connect(deviceName1,pin1,deviceName2,pin2)){
		wcout<<"following filters are connected "<<endl;
		wcout<<"\t("<<deviceName1<<","<<pin1<<") "<<endl;
		wcout<<"\t("<<deviceName2<<","<<pin2<<") "<<endl;
	}else{
		wcout<<"failed to connect filters : "<<pDxDev->errorMsg()<<endl;
		return 1;
	}

	//Now run the graph
	if(pDxDev->run()) {
		wcout<<_T("Now running...")<<endl;
	}else{
		wcout<<_T("failed to run :")<<pDxDev->errorMsg()<<endl;
		return 1;
	}

	wcout<<"Close the window to exit or hit any key"<<endl;
	while(!_kbhit())
	{
	}
	delete pDxDev;

	CoUninitialize();
}