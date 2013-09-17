#include<iostream>
#include<cstring>
#include <aCapture/CDxDev.h>
#include <aCapture/CDxHelper.h>
#include<conio.h>

void HR_Failed(HRESULT hr);// hr status function
IBaseFilter* Device_Init(IMoniker*);//Function to initialize Input/Output devices

void Device_Addition(IGraphBuilder*,IBaseFilter*,String);//Function to add device to graph
void Device_Connect(IBaseFilter*,IBaseFilter*);//Function to connect the two devices, in this case input and output
void Run_Graph(IMediaControl*);//Function to run the graph

using namespace std;


int main (void)
{
	HRESULT hr;	// COM result
	IGraphBuilder *pGraph = NULL;// Main graphbuilder pointer
	IMediaControl *pControl = NULL;	// Media Control interface
	ICreateDevEnum *pDeviceEnum = NULL;// System device enumerator
	IBaseFilter *pInputDevice = NULL, *pOutputDevice = NULL;// Input and output filters(devices)
	IMoniker *pDeviceMonik = NULL;// Device moniker
	GUID DEVICE_CLSID ;// GUID i.e.  CLSID_Xxxxxxxxxxx
	String deviceName;

	hr = CoInitialize(NULL);// Initialise COM
	if (FAILED(hr))
	{
		HR_Failed(hr);
		return hr;
	}

	CDxDev* pDxDev = new CDxDev();

	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,IID_IGraphBuilder, (void**)&pGraph);//Initialize Graph builder
	if (FAILED(hr))
	{
		HR_Failed(hr);
		return hr;
	}
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,IID_ICreateDevEnum, (void **)&pDeviceEnum);//Initialize Device enumerator
	if (FAILED(hr))
	{
		HR_Failed(hr);
		return hr;
	}
	hr = pGraph->QueryInterface(IID_IMediaControl,(void**)&pControl);// Query interface for IMediaControl
	if (FAILED(hr))
	{
		HR_Failed(hr);
		return hr;
	}
	/**********************************************************************************/
	//Front mic input
	DEVICE_CLSID = CLSID_AudioInputDeviceCategory;// the input device category
	deviceName = L"마이크(Realtek High Definition Aud";
	pDeviceMonik = CDxHelper::read(pDeviceEnum,DEVICE_CLSID,deviceName);//read the required device 
	pInputDevice = Device_Init(pDeviceMonik);//Return the device after initializing it
	Device_Addition(pGraph,pInputDevice,deviceName);//add device to graph

	/******************************************************************************/
	//Default output device
	DEVICE_CLSID = CLSID_AudioRendererCategory;// the audio renderer device category
	deviceName = L"스피커(Realtek High Definition Aud";// device name as seen in Graphedit.exe
	pDeviceMonik = CDxHelper::read(pDeviceEnum,DEVICE_CLSID,deviceName);//read the required device
	pOutputDevice = Device_Init(pDeviceMonik);//Return the device after initializing it
	Device_Addition(pGraph,pOutputDevice,deviceName);//add device to graph
	/******************************************************************************/
	//Connect input to output
	Device_Connect(pInputDevice,pOutputDevice);
	/*******************************************************************************/
	//Now run the graph
	Run_Graph(pControl);

	//Loop till you don't close the console window or hit a key!
	cout<<"Close the window to exit or hit any key"<<endl;
	while(!_kbhit())
	{
	}
	pControl->Release();//Release control
	pDeviceEnum->Release();//Release Device enumerator
	pGraph->Release();//Release the Graph

	delete pDxDev;
}

void HR_Failed(HRESULT hr)
{
	TCHAR szErr[MAX_ERROR_TEXT_LEN];
	DWORD res = AMGetErrorText(hr, szErr, MAX_ERROR_TEXT_LEN);
	if (res == 0)
	{
		StringCchPrintf(szErr, MAX_ERROR_TEXT_LEN, L"Unknown Error: 0x%2x", hr);
	}

	MessageBox(0, szErr, TEXT("Error!"), MB_OK | MB_ICONERROR);
	return;
}

IBaseFilter* Device_Init(IMoniker* pDeviceMonik)
{
	IBaseFilter* pDevice = NULL;
	HRESULT hr;
	hr = pDeviceMonik->BindToObject(NULL, NULL, IID_IBaseFilter,(void**)&pDevice);//Instantiate the device
	if (SUCCEEDED(hr))
	{
		cout<<"Device initiation successful..."<<endl;
	}
	else HR_Failed(hr);

	return pDevice;
}

void Device_Addition(IGraphBuilder* pGraph,IBaseFilter* pDevice,String name)
{
	HRESULT hr;
	hr = pGraph->AddFilter(pDevice,name.c_str());
	if(SUCCEEDED(hr))
	{
		wcout<<"Addition of "<<name<<" successful..."<<endl;
	}
	else HR_Failed(hr);
}

void Device_Connect(IBaseFilter* pInputDevice,IBaseFilter* pOutputDevice)
{
	HRESULT hr;
	IEnumPins *pInputPin = NULL,*pOutputPin  = NULL;// Pin enumeration
	IPin *pIn = NULL, *pOut = NULL;// Pins

	hr = pInputDevice->EnumPins(&pInputPin);// Enumerate the pin
	if(SUCCEEDED(hr))
	{
		cout<<"Input Pin Enumeration successful..."<<endl;
		hr = pInputDevice->FindPin(L"Capture",&pIn);//Get hold of the pin "Capture", as seen in GraphEdit
		if(SUCCEEDED(hr))
		{
			cout<<"Capture pin found"<<endl;	
		}
		else HR_Failed(hr);

	}
	else HR_Failed(hr);

	hr = pOutputDevice->EnumPins(&pOutputPin);//Enumerate the pin
	if(SUCCEEDED(hr))
	{
		cout<<"Output Pin Enumeration successful..."<<endl;
		hr = pOutputDevice->FindPin(L"Audio Input pin (rendered)",&pOut);
		if(SUCCEEDED(hr))
		{
			cout<<"Audio Input Pin (rendered) found"<<endl;
		}
		else HR_Failed(hr);

	}
	else HR_Failed(hr);

	hr = pIn->Connect(pOut,NULL);	//Connect the input pin to output pin
	if(SUCCEEDED(hr))
	{
		cout<<"Pin connection successful..."<<endl;
	}
	else HR_Failed(hr);

}

void Run_Graph(IMediaControl* pControl)
{
	HRESULT hr;
	hr = pControl->Run();// Now run the graph, i.e. start listening!
	if(SUCCEEDED(hr))
	{
		cout<<"You must be listening to something!!!"<<endl;
	}
	else HR_Failed(hr);
}
