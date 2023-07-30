#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/dlib/android.h>

namespace dmPermissions {

    void Initialize_Ext();

    int Check(const char *permission);
    void Request(const char **permissions, int len);

} //namespace dmPermissions

#endif // DM_PLATFORM_ANDROID
