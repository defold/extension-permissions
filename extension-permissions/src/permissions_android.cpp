#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/dlib/android.h>
#include "permissions_private.h"
#include "permissions_callback_private.h"

namespace dmPermissions {

    struct Permissions
    {
        jobject        m_PermissionsJNI;

        jmethodID      m_Check;

    };

    static Permissions       g_permissions;

    static int CallIntMethodChar(jobject instance, jmethodID method, const char* cstr)
    {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        jstring jstr = env->NewStringUTF(cstr);
        jint return_value = (jint)env->CallIntMethod(instance, method, jstr);
        env->DeleteLocalRef(jstr);
        return (int)return_value;
    }

    static void InitJNIMethods(JNIEnv* env, jclass cls)
    {
        g_permissions.m_Check = env->GetMethodID(cls, "check", "(Ljava/lang/String;)I");
    }

    void Initialize_Ext()
    {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        jclass cls = dmAndroid::LoadClass(env, "com.defold.permissions.PermissionsJNI");

        InitJNIMethods(env, cls);

        jmethodID jni_constructor = env->GetMethodID(cls, "<init>", "(Landroid/app/Activity;)V");

        g_permissions.m_PermissionsJNI = env->NewGlobalRef(env->NewObject(cls, jni_constructor, threadAttacher.GetActivity()->clazz));
    }

    int Check(const char* permission)
    {
        int result = CallIntMethodChar(g_permissions.m_PermissionsJNI, g_permissions.m_Check, permission);
        return result;
    }

} //namespace dmPermissions

#endif // DM_PLATFORM_ANDROID
