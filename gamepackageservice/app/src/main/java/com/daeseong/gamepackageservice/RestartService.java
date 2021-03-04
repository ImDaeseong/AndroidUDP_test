package com.daeseong.gamepackageservice;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.util.Log;

public class RestartService extends BroadcastReceiver {

    private static final String TAG = RestartService.class.getSimpleName();

    @Override
    public void onReceive(Context context, Intent intent) {

        Log.d(TAG, "onReceive:" + intent.getAction().toString());

        try {

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                Intent service = new Intent(context, GamePackageService .class);
                context.startForegroundService(service);
            } else {
                Intent service = new Intent(context, GamePackageService .class);
                context.startService(service);
            }

        }catch (Exception ex){
            Log.e(TAG, ex.getMessage().toString());
        }
    }

}