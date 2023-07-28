#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/dlib/android.h>

namespace dmPermissions {

    void Initialize_Ext();

    int Check(const char* permission);

} //namespace dmPermissions

#endif // DM_PLATFORM_ANDROID
