#include <aCapture/CDxHelper.h>

IMoniker* CDxHelper::read(ICreateDevEnum* pDeviceEnum,GUID devClsid,String deviceName)
{
	IEnumMoniker *pEnumCat = NULL;// Device enumeration moniker
	VARIANT varName;
	IMoniker *pDeviceMonik = NULL;

	HRESULT hr = pDeviceEnum->CreateClassEnumerator(devClsid, &pEnumCat, 0);// Enumerate the specified device, distinguished by DEVICE_CLSID

	if (hr == S_OK) 
	{
		ULONG cFetched;
		while (pEnumCat->Next(1, &pDeviceMonik, &cFetched) == S_OK)//Pickup as moniker
		{
			IPropertyBag *pPropBag = NULL;
			hr = pDeviceMonik->BindToStorage(0, 0, IID_IPropertyBag,(void **)&pPropBag);//bind the properties of the moniker
			if (SUCCEEDED(hr))
			{
				VariantInit(&varName);// Initialise the variant data type
				hr = pPropBag->Read(L"FriendlyName", &varName, 0);
				if (SUCCEEDED(hr))
				{	
					if(String(varName.bstrVal)==deviceName)
						return pDeviceMonik;
				}
				VariantClear(&varName);//clear the variant data type
				pPropBag->Release();//release the properties
			}
			pDeviceMonik->Release();//release Device moniker
		}
		pEnumCat->Release();//release category enumerator
	}
	return NULL;
}