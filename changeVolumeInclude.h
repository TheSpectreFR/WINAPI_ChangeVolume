#ifndef _WINDOWS_
#include <windows.h>
#endif
#ifndef INITGUID
#include <initguid.h>
#endif
#ifndef __mmdeviceapi_h__
#include <mmdeviceapi.h>
#endif
#ifndef __endpointvolume_h__
#include <endpointvolume.h>
#endif

int changeVolume(float paramNewVolume)
{
    HRESULT hresult = 0;

    float newVolume = paramNewVolume; // needs to be a 0.XXf floating point type, see example in main function

    IMMDeviceEnumerator *deviceEnum = NULL;
    IMMDevice *defaultDevice = NULL;
    IAudioEndpointVolume *endpointVolume = NULL;

    CoInitialize(NULL);

    hresult=CoCreateInstance(&CLSID_MMDeviceEnumerator, 0, CLSCTX_INPROC_SERVER, &IID_IMMDeviceEnumerator, (void **)&deviceEnum);
    if(hresult==0)
    {
        if(deviceEnum)
        {
            deviceEnum->lpVtbl->GetDefaultAudioEndpoint(deviceEnum, eRender, eConsole, &defaultDevice);
            deviceEnum->lpVtbl->Release(deviceEnum);
        }
        if(defaultDevice)
        {
            defaultDevice->lpVtbl->Activate(defaultDevice, &IID_IAudioEndpointVolume, CLSCTX_INPROC_SERVER, 0, (void **)&endpointVolume);
            defaultDevice->lpVtbl->Release(defaultDevice);
        }
        if(endpointVolume)
        {
            endpointVolume->lpVtbl->SetMasterVolumeLevelScalar(endpointVolume, (float)newVolume, 0);
            endpointVolume->lpVtbl->Release(endpointVolume);
        }
    }
    CoUninitialize();
    return 0;
}