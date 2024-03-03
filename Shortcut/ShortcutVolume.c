/*
    Simple program made to tweak windows general volume using
    key combination: CTRL+ALT+UP/DOWN-ARROW

    NOTE:
    this version contains a side effect !
    if you press up/down-arrow before pressing CTRL and ALT keys,
    the program does change the volume even if you are not pressing
    arrows anymore...
    -> needs a patch.
*/
#include <stdio.h>
#include <windows.h>
#include <initguid.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
DWORD WINAPI isBelowFiftyPercent(void *parameters);
int changeVolume(float param);
int RUN;
float GLOBAL_VOLUME;

int main(int argc, char *argv[])
{
    RUN = 1;
    GLOBAL_VOLUME = 0.0f;
    changeVolume(GLOBAL_VOLUME);

    DWORD ThreadID;
    HANDLE Thread = CreateThread(0, 0, isBelowFiftyPercent, NULL, 0, &ThreadID);

    while(RUN == 1)
    {
        if(GetKeyState(0x11)&0x1000) // if CTRL key pressed
        {
            if(GetKeyState(0x12)&0x1000) // if ALT key pressed
            {
                if(GetAsyncKeyState(0x26)&0x0001) // if UP-ARROW key pressed
                {
                    GLOBAL_VOLUME += 0.02f;
                    changeVolume(GLOBAL_VOLUME);
                    printf("Increased volume to %f\n", GLOBAL_VOLUME);
                }else if(GetAsyncKeyState(0x28)&0x0001) // if DOWN-ARROW key pressed
                {
                    if(GLOBAL_VOLUME <= 0.03f)
                    {
                        GLOBAL_VOLUME = 0.01f;
                        changeVolume(GLOBAL_VOLUME);
                    }else
                    {
                        GLOBAL_VOLUME -= 0.03f;
                        changeVolume(GLOBAL_VOLUME);
                    }
                    printf("Decreased volume to %f\n", GLOBAL_VOLUME);
                }
            }
        }
        Sleep(100);
    }
}
int changeVolume(float param)
{
    HRESULT hresult=0;

    float newVolume = param;

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
DWORD WINAPI isBelowFiftyPercent(void *parameters)
{
    while(RUN == 1)
    {
        if(GLOBAL_VOLUME > 0.5f)
        {
            GLOBAL_VOLUME = 0.46f;
        }
        Sleep(100);
    }
}