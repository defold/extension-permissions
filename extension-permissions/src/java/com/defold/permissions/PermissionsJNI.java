package com.defold.permissions;

import android.os.Build;
import android.os.Bundle;
import android.app.Activity;
import android.content.pm.PackageManager;
import androidx.core.content.ContextCompat;
import androidx.core.app.ActivityCompat;
import android.app.Fragment;
import android.app.FragmentManager;
import android.app.FragmentTransaction;

import org.json.JSONObject;
import org.json.JSONException;

import java.util.Map;

public class PermissionsJNI {
    private static final String TAG = "PermissionsJNI";
    private static final int REQUEST_CODE = 6894;

    public static native void permissionsAddToQueue(String json);

    // duplicate of enums from permissions_callback_private.h:
    // CONSTANTS:
    private static final int PERMISSION_GRANTED =         1;
    private static final int PERMISSION_DENIED =          2;
    private static final int PERMISSION_SHOW_RATIONALE =  3;

    private Activity activity;
    private PermissionsFragment permissionsFragment;

    public PermissionsJNI(Activity activity) {
        this.activity = activity;
        this.permissionsFragment = new PermissionsFragment();
        activity.getFragmentManager().beginTransaction().add(permissionsFragment, PermissionsFragment.TAG).commit();
    }

    public int check(String permission) {
        if (ContextCompat.checkSelfPermission(activity.getApplicationContext(), permission) == PackageManager.PERMISSION_GRANTED) {
            // You can use the API that requires the permission.
            return PERMISSION_GRANTED;
        }
        else if (ActivityCompat.shouldShowRequestPermissionRationale(activity, permission)) {
            // In an educational UI, explain to the user why your app requires this
            // permission for a specific feature to behave as expected, and what
            // features are disabled if it's declined. In this UI, include a
            // "cancel" or "no thanks" button that lets the user continue
            // using your app without granting the permission.
            return PERMISSION_SHOW_RATIONALE;
        }
        else {
            // You can directly ask for the permission.
            return PERMISSION_DENIED;
        }
    }

    public void request(final String[] permissions) {
        if (Build.VERSION.SDK_INT < 23) {
            String message = null;
            try {
                JSONObject obj = new JSONObject();
                for (String permission : permissions) {
                    obj.put(permission, check(permission));
                }
                message = obj.toString();
            }
            catch (JSONException e) {
                message = getJsonConversionErrorMessage(e.getLocalizedMessage());
            }
            permissionsAddToQueue(message);
        }
        else {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    permissionsFragment.requestPermissions(permissions, REQUEST_CODE);
                }
            });
        }
    }

    // https://www.baeldung.com/java-json-escaping
    private static String getJsonConversionErrorMessage(String messageText) {
        String message = null;
        try {
            JSONObject obj = new JSONObject();
            obj.put("error", messageText);
            message = obj.toString();
        }
        catch (JSONException e) {
            message = "{ \"error\": \"Error while converting message to JSON.\" }";
        }
        return message;
    }

    public static class PermissionsFragment extends Fragment {
        public static final String TAG = "PermissionsFragment";

        @Override
        public void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            setRetainInstance(true);
        }
       
        @Override
        public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
            if (requestCode == REQUEST_CODE) {
                String message = null;
                try {
                    JSONObject obj = new JSONObject();
                    for (int i = 0; i < permissions.length; i++) {
                        obj.put(permissions[i], grantResults[i] == PackageManager.PERMISSION_GRANTED ? PERMISSION_GRANTED : PERMISSION_DENIED);
                    }
                    message = obj.toString();
                }
                catch (JSONException e) {
                    message = getJsonConversionErrorMessage(e.getLocalizedMessage());
                }
                permissionsAddToQueue(message);
            }
        }
    }
}
