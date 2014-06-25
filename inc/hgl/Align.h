#ifndef HGL_GUI_ALIGN_INCLUDE
#define HGL_GUI_ALIGN_INCLUDE

namespace hgl
{
	/**
	* 对齐方式
	*/
	enum VAlign
	{
		alNone,                     ///<起始定义，无意义

		alTop,                      ///<对齐最上端
		alBottom,                   ///<对齐底端
		alLeft,                     ///<对齐左边
		alRight,                    ///<对齐右边

		alClient,                   ///<使用剩下的所有空间

		alLeftTop,                  ///<左上角
		alRightTop,                 ///<右上角
		alLeftBottom,               ///<左下角
		alRightBottom,              ///<右下角

		alTopCenter,                ///<上边中间
		alLeftCenter,               ///<左边中间
		alRightCenter,              ///<右边中间
		alBottomCenter,             ///<下边中间

		alCenter,                   ///<正中间

		alEnd                       ///<结束定义，无意义
	};

    /**
    * 文本排列方式枚举
    */
    enum TextAlignment			///文本排列方式
    {
        taNone=0,				///<起始定义，无意义。如使用处理为距左

        taLeft,					///<距左
        taCenter,				///<距中
		taRight,				///<距右

		taVRight,				///<距右(竖排)

        taEnd					///<结束定义，无意义。如使用处理为距左
	};
}//namespace hgl
#endif//HGL_GUI_ALIGN_INCLUDE
