#if defined(DM_PLATFORM_ANDROID)

#include <jni.h>
/* Header for class com_defold_permissions_PermissionsJNI */

#ifndef COM_DEFOLD_PERMISSIONS_PERMISSIONSJNI_H
#define COM_DEFOLD_PERMISSIONS_PERMISSIONSJNI_H
#ifdef __cplusplus
extern "C" {
#endif
	/*
	* Class:     com_defold_permissions_PermissionsJNI
	* Method:    permissionsAddToQueue
	* Signature: (Ljava/lang/String;)V
	*/
	JNIEXPORT void JNICALL Java_com_defold_permissions_PermissionsJNI_permissionsAddToQueue
		(JNIEnv *, jclass, jstring);

#ifdef __cplusplus
}
#endif
#endif

#endif
