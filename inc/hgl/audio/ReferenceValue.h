#ifndef HGL_AUDIO_REFERENCE_VALUE_INCLUDE
#define HGL_AUDIO_REFERENCE_VALUE_INCLUDE

namespace hgl
{
	class ReferenceValue				///参考量
	{
	public:

		float half;     				///<半衰距离
		float max;      				///<最大距离

	public:

		ReferenceValue()
		{
			half=0;
			max=0;
		}
	};//class ReferenceValue
}//namespace hgl
#endif//HGL_AUDIO_REFERENCE_VALUE_INCLUDE
