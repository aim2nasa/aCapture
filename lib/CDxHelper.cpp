#include <aCapture/CDxHelper.h>

HRESULT CDxHelper::read(ICreateDevEnum* pDeviceEnum,GUID devClsid,String deviceName,IMoniker** ppOutMoniker)
{
	*ppOutMoniker = NULL;
	IEnumMoniker *pEnumCat = NULL;// Device enumeration moniker
	HRESULT hr = pDeviceEnum->CreateClassEnumerator(devClsid, &pEnumCat, 0);// Enumerate the specified device, distinguished by DEVICE_CLSID

	if (hr == S_OK) 
	{
		ULONG cFetched;
		IMoniker *pDeviceMonik = NULL;
		while (pEnumCat->Next(1, &pDeviceMonik, &cFetched) == S_OK)//Pickup as moniker
		{
			IPropertyBag *pPropBag = NULL;
			hr = pDeviceMonik->BindToStorage(0, 0, IID_IPropertyBag,(void **)&pPropBag);//bind the properties of the moniker
			if (SUCCEEDED(hr))
			{
				VARIANT varName;
				VariantInit(&varName);// Initialise the variant data type
				hr = pPropBag->Read(L"FriendlyName", &varName, 0);
				if (SUCCEEDED(hr))
				{	
					if(String(varName.bstrVal)==deviceName) {
						*ppOutMoniker = pDeviceMonik;
						return S_OK;
					}
				}
				VariantClear(&varName);//clear the variant data type
				pPropBag->Release();//release the properties
			}
			pDeviceMonik->Release();//release Device moniker
		}
		pEnumCat->Release();//release category enumerator
	}
	return E_FAIL;
}

HRESULT CDxHelper::bind(IMoniker* pDeviceMonik,IBaseFilter** ppOutFilter)
{
	*ppOutFilter = NULL;
	HRESULT hr = pDeviceMonik->BindToObject(NULL, NULL, IID_IBaseFilter,(void**)ppOutFilter);	//Instantiate the device
	return hr;
}

HRESULT CDxHelper::addToGraph(IGraphBuilder* pGraph,IBaseFilter* pDevice,String name)
{
	return pGraph->AddFilter(pDevice,name.c_str());
}

HRESULT CDxHelper::getPin(IBaseFilter* pDevice,String pinName,IPin** ppOutPin)
{
	*ppOutPin = NULL;
	return pDevice->FindPin(pinName.c_str(),ppOutPin);
}

HRESULT CDxHelper::connect(IPin* pPin1,IPin* pPin2)
{
	return pPin1->Connect(pPin2,NULL);
}

HRESULT CDxHelper::run(IMediaControl* pCtrl)
{
	return pCtrl->Run();
}

HRESULT CDxHelper::conFilter(IBaseFilter* pInpDev,String inpPinName,IBaseFilter* pOutDev,String outPinName)
{
	IPin *pIn,*pOut;

	HRESULT hr = CDxHelper::getPin(pInpDev,inpPinName,&pIn);
	if(FAILED(hr)) return hr;

	hr = CDxHelper::getPin(pOutDev,outPinName,&pOut);
	if(FAILED(hr)) return hr;

	return CDxHelper::connect(pIn,pOut);
}