#include<hgl/al/xram.h>
#include<hgl/al/alc.h>

namespace openal
{
    // XRAM functions and enum values

    LPEAXSETBUFFERMODE eaxSetBufferMode = 0;
    LPEAXGETBUFFERMODE eaxGetBufferMode = 0;

    ALenum eXRAMSize = 0;
    ALenum eXRAMFree = 0;
    ALenum eXRAMAuto = 0;
    ALenum eXRAMHardware = 0;
    ALenum eXRAMAccessible = 0;
}

namespace openal
{
    bool CheckXRAM(ALCdevice_struct *AudioDevice)
    {
        if(alcIsExtensionPresent(AudioDevice, "EAX-RAM"))
        {
            if (eaxSetBufferMode && eaxGetBufferMode)
            {
                eXRAMSize = alGetEnumValue("AL_EAX_RAM_SIZE");
                eXRAMFree = alGetEnumValue("AL_EAX_RAM_FREE");
                eXRAMAuto = alGetEnumValue("AL_STORAGE_AUTOMATIC");
                eXRAMHardware = alGetEnumValue("AL_STORAGE_HARDWARE");
                eXRAMAccessible = alGetEnumValue("AL_STORAGE_ACCESSIBLE");

                if (eXRAMSize && eXRAMFree && eXRAMAuto && eXRAMHardware && eXRAMAccessible)
                    return(true);
            }
        }

        return(false);
    }

    void ClearXRAM()
    {
        eaxSetBufferMode = 0;
        eaxGetBufferMode = 0;

        eXRAMSize = 0;
        eXRAMFree = 0;
        eXRAMAuto = 0;
        eXRAMHardware = 0;
        eXRAMAccessible = 0;
    }
}//namespace openal
