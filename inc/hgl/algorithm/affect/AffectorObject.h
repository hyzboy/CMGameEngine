#ifndef HGL_ALGORITHM_AFFECTOR_OBJECT_INCLUDE
#define HGL_ALGORITHM_AFFECTOR_OBJECT_INCLUDE

#include<hgl/object/Object.h>
namespace hgl
{
	class TDFNode;

    namespace algorithm
    { 
        namespace affect
        {
	        class AffectorControl;

	        /**
	        * 影响器对像
	        */
            class AffectorObject                                                                    ///<影响器对像
	        {
	        protected:

		        AffectorControl *control;															///<控制器

	        public:	//方法

		        AffectorObject(){control=nullptr;}													///<本类构造函数
		        virtual ~AffectorObject()=default;													///<本类析构函数

		        virtual void SetControl(AffectorControl *ac){control=ac;}							///<设置控制器

		        virtual const char *GetClassName(){return nullptr;}								    ///<取得类名

		        virtual bool Start(){return(true);}													///<开始
		        virtual bool Pause(){return(true);}													///<暂停
		        virtual bool Resume(){return(true);}												///<恢复
		        virtual bool Stop(){return(true);}													///<停止

		        virtual void Update(){};															///<刷新
	        };//class AffectorObject
        }//namespace affect
    }//namespace algorithm
}//namespace hgl
#endif//HGL_ALGORITHM_AFFECTOR_OBJECT_INCLUDE
