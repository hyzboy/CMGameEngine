#include<hgl/platform/Platform.h>
#include<jni.h>

namespace hgl
{
    namespace logger
    {
        void SetLocalAppdataPath(const char *fn);
    }//namespace logger

    void InitAndroidSupport(JNIEnv *env,jobject obj)
    {
        jclass      cls             =env->GetObjectClass(obj);
        jmethodID   getFilesDir     =env->GetMethodID(cls, "getFilesDir", "()Ljava/io/File;");
        jobject     dirobj          =env->CallObjectMethod(obj,getFilesDir);
        jclass      dir             =env->GetObjectClass(dirobj);
        jmethodID   getStoragePath  =env->GetMethodID(dir, "getAbsolutePath", "()Ljava/lang/String;");
        jstring     path            =(jstring)env->CallObjectMethod(dirobj, getStoragePath);
        const char *pathstr         =env->GetStringUTFChars(path, 0);

        logger::SetLocalAppdataPath(pathstr);

        env->ReleaseStringUTFChars(path, pathstr);
    }
}//namespace hgl
