#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/dlib/android.h>

namespace dmPermissions {

    // The same constants/enums are in PermissionsJNI.java
    // If you change enums here, pls make sure you update the constants there as well
    enum PermissionsResults
    {
        PERMISSION_GRANTED = 1,
        PERMISSION_DENIED = 2,
        PERMISSION_SHOW_RATIONALE = 3
    };

} //namespace dmPermissions

#endif // DM_PLATFORM_ANDROID
