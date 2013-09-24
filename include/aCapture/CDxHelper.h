#ifndef __CDXHELPER_H__
#define __CDXHELPER_H__

#include <aCapture/typedef.h>

class DLLEXP CDxHelper{
public:
	static HRESULT read(ICreateDevEnum* pDeviceEnum,GUID devClsid,String deviceName,IMoniker** ppOutMoniker);
	static HRESULT bind(IMoniker* pDeviceMonik,IBaseFilter** ppOutFilter);
	static HRESULT addToGraph(IGraphBuilder* pGraph,IBaseFilter* pDevice,String name);
	static HRESULT getPin(IBaseFilter* pDevice,String pinName,IPin** ppOutPin);
	static HRESULT connect(IPin* pPin1,IPin* pPin2);
	static HRESULT run(IMediaControl* pCtrl);
	static HRESULT conFilter(IBaseFilter* pInpDev,String inpPinName,IBaseFilter* pOutDev,String outPinName);
};

#endif