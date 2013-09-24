#include<iostream>
#include<cstring>
#include <aCapture/CDxDev.h>
#include <aCapture/CDxHelper.h>
#include<conio.h>

void HR_Failed(HRESULT hr);// hr status function

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
	hr = CDxHelper::read(pDeviceEnum,DEVICE_CLSID,deviceName,&pDeviceMonik);//read the required device 
	if (FAILED(hr))
	{
		HR_Failed(hr);
		return hr;
	}
	pInputDevice = CDxHelper::bind(pDeviceMonik);//Return the device after initializing it
	CDxHelper::addToGraph(pGraph,pInputDevice,deviceName);//add device to graph

	/******************************************************************************/
	//Default output device
	DEVICE_CLSID = CLSID_AudioRendererCategory;// the audio renderer device category
	deviceName = L"스피커(Realtek High Definition Aud";// device name as seen in Graphedit.exe
	hr = CDxHelper::read(pDeviceEnum,DEVICE_CLSID,deviceName,&pDeviceMonik);//read the required device
	if (FAILED(hr))
	{
		HR_Failed(hr);
		return hr;
	}
	pOutputDevice = CDxHelper::bind(pDeviceMonik);//Return the device after initializing it
	CDxHelper::addToGraph(pGraph,pOutputDevice,deviceName);//add device to graph

	/******************************************************************************/
	//Connect input to output
	if(SUCCEEDED(CDxHelper::conFilter(pInputDevice,String(L"Capture"),pOutputDevice,String(L"Audio Input pin (rendered)"))))
		cout<<"Two filters are connected"<<endl;

	/*******************************************************************************/
	//Now run the graph
	if(SUCCEEDED(CDxHelper::run(pControl)))
		cout<<"You must be listening to something!!!"<<endl;

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