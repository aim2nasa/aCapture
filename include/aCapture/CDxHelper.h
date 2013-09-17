#ifndef __CDXHELPER_H__
#define __CDXHELPER_H__

#include <aCapture/typedef.h>
#include <dshow.h>

class DLLEXP CDxHelper{
public:
	static IMoniker* read(ICreateDevEnum* pDeviceEnum,GUID devClsid,String deviceName);
	static IBaseFilter* bind(IMoniker* pDeviceMonik);
};

#endif