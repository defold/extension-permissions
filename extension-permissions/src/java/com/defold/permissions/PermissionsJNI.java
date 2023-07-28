package com.defold.permissions;

import android.app.Activity;
import android.content.pm.PackageManager;
import androidx.core.content.ContextCompat;
import androidx.core.app.ActivityCompat;

public class PermissionsJNI {
    private static final String TAG = "PermissionsJNI";

    // duplicate of enums from permissions_callback_private.h:
    // CONSTANTS:
    private static final int PERMISSION_GRANTED =         1;
    private static final int PERMISSION_DENIED =          2;
    private static final int PERMISSION_SHOW_RATIONALE =  3;

    private Activity activity;

    public PermissionsJNI(Activity activity) {
        this.activity = activity;
    }

    public int check(String permission) {
        if (ContextCompat.checkSelfPermission(activity.getApplicationContext(), permission) == PackageManager.PERMISSION_GRANTED) {
            // You can use the API that requires the permission.
            return PERMISSION_GRANTED;
        } else if (ActivityCompat.shouldShowRequestPermissionRationale(activity, permission)) {
            // In an educational UI, explain to the user why your app requires this
            // permission for a specific feature to behave as expected, and what
            // features are disabled if it's declined. In this UI, include a
            // "cancel" or "no thanks" button that lets the user continue
            // using your app without granting the permission.
            return PERMISSION_SHOW_RATIONALE;
        } else {
            // You can directly ask for the permission.
            return PERMISSION_DENIED;
        }
    }
}
