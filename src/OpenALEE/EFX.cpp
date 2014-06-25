#include<hgl/al/efx.h>
#include<hgl/al/alc.h>
#include<hgl/ExternalModule.h>

using namespace hgl;

namespace openal
{
	// Imported EFX functions

	// Effect objects
	LPALGENEFFECTS alGenEffects = 0;
	LPALDELETEEFFECTS alDeleteEffects = 0;
	LPALISEFFECT alIsEffect = 0;
	LPALEFFECTI alEffecti = 0;
	LPALEFFECTIV alEffectiv = 0;
	LPALEFFECTF alEffectf = 0;
	LPALEFFECTFV alEffectfv = 0;
	LPALGETEFFECTI alGetEffecti = 0;
	LPALGETEFFECTIV alGetEffectiv = 0;
	LPALGETEFFECTF alGetEffectf = 0;
	LPALGETEFFECTFV alGetEffectfv = 0;

	//Filter objects
	LPALGENFILTERS alGenFilters = 0;
	LPALDELETEFILTERS alDeleteFilters = 0;
	LPALISFILTER alIsFilter = 0;
	LPALFILTERI alFilteri = 0;
	LPALFILTERIV alFilteriv = 0;
	LPALFILTERF alFilterf = 0;
	LPALFILTERFV alFilterfv = 0;
	LPALGETFILTERI alGetFilteri = 0;
	LPALGETFILTERIV alGetFilteriv = 0;
	LPALGETFILTERF alGetFilterf = 0;
	LPALGETFILTERFV alGetFilterfv = 0;

	// Auxiliary slot object
	LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots = 0;
	LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots = 0;
	LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot = 0;
	LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti = 0;
	LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv = 0;
	LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf = 0;
	LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv = 0;
	LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti = 0;
	LPALGETAUXILIARYEFFECTSLOTIV alGetAuxiliaryEffectSlotiv = 0;
	LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf = 0;
	LPALGETAUXILIARYEFFECTSLOTFV alGetAuxiliaryEffectSlotfv = 0;
}

namespace openal
{
	bool CheckEFX(ALCdevice_struct *AudioDevice)
	{
		HGL_FUNC_LOAD_LIST_BEGIN(al_efx_func_load_list)
			HGL_FUNC_LOAD(alGenEffects)
			HGL_FUNC_LOAD(alDeleteEffects)
			HGL_FUNC_LOAD(alIsEffect)
			HGL_FUNC_LOAD(alEffecti)
			HGL_FUNC_LOAD(alEffectiv)
			HGL_FUNC_LOAD(alEffectf)
			HGL_FUNC_LOAD(alEffectfv)
			HGL_FUNC_LOAD(alGetEffecti)
			HGL_FUNC_LOAD(alGetEffectiv)
			HGL_FUNC_LOAD(alGetEffectf)
			HGL_FUNC_LOAD(alGetEffectfv)

			HGL_FUNC_LOAD(alGenFilters)
			HGL_FUNC_LOAD(alDeleteFilters)
			HGL_FUNC_LOAD(alIsFilter)
			HGL_FUNC_LOAD(alFilteri)
			HGL_FUNC_LOAD(alFilteriv)
			HGL_FUNC_LOAD(alFilterf)
			HGL_FUNC_LOAD(alFilterfv)
			HGL_FUNC_LOAD(alGetFilteri)
			HGL_FUNC_LOAD(alGetFilteriv)
			HGL_FUNC_LOAD(alGetFilterf)
			HGL_FUNC_LOAD(alGetFilterfv)

			HGL_FUNC_LOAD(alGenAuxiliaryEffectSlots)
			HGL_FUNC_LOAD(alDeleteAuxiliaryEffectSlots)
			HGL_FUNC_LOAD(alIsAuxiliaryEffectSlot)
			HGL_FUNC_LOAD(alAuxiliaryEffectSloti)
			HGL_FUNC_LOAD(alAuxiliaryEffectSlotiv)
			HGL_FUNC_LOAD(alAuxiliaryEffectSlotf)
			HGL_FUNC_LOAD(alAuxiliaryEffectSlotfv)
			HGL_FUNC_LOAD(alGetAuxiliaryEffectSloti)
			HGL_FUNC_LOAD(alGetAuxiliaryEffectSlotiv)
			HGL_FUNC_LOAD(alGetAuxiliaryEffectSlotf)
			HGL_FUNC_LOAD(alGetAuxiliaryEffectSlotfv)
		HGL_FUNC_LOAD_LIST_END

		if (alcIsExtensionPresent(AudioDevice, (ALCchar*)ALC_EXT_EFX_NAME))
		{
			FuncLoad *flp=al_efx_func_load_list;

			while(flp->func_pointer)
			{
				(*(flp->func_pointer))=alGetProcAddress(flp->func_name);

				if(!(*(flp->func_pointer)))
					return(false);

				++flp;
			}

			return(true);
		}

		return(false);
	}

	void ClearEFX()
	{
// 		ClearFuncLoadPointer(al_efx_func_load_list);
	}
}//namespace openal
