#if defined(DM_PLATFORM_ANDROID)

#include "permissions_private.h"
#include "permissions_callback_private.h"
#include "com_defold_permissions_PermissionsJNI.h"

JNIEXPORT void JNICALL Java_com_defold_permissions_PermissionsJNI_permissionsAddToQueue(JNIEnv * env, jclass cls, jstring jjson)
{
    const char* json = env->GetStringUTFChars(jjson, 0);
    dmPermissions::AddToQueueCallback(json);
    env->ReleaseStringUTFChars(jjson, json);
}

namespace dmPermissions {

    struct Permissions
    {
        jobject        m_PermissionsJNI;

        jmethodID      m_Check;
        jmethodID      m_Request;

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
        g_permissions.m_Request = env->GetMethodID(cls, "request", "([Ljava/lang/String;)V");
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

    void Request(const char **permissions, int len)
    {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();

        jobjectArray jPermissions = (jobjectArray)env->NewObjectArray(len, env->FindClass("java/lang/String"), NULL);
        for (int i = 0; i < len; i++)
        {
            jstring jPermission = env->NewStringUTF(permissions[i]);
            env->SetObjectArrayElement(jPermissions, i, jPermission);
            env->DeleteLocalRef(jPermission);
        }
        env->CallVoidMethod(g_permissions.m_PermissionsJNI, g_permissions.m_Request, jPermissions);
        env->DeleteLocalRef(jPermissions);
    }

} //namespace dmPermissions

#endif // DM_PLATFORM_ANDROID
