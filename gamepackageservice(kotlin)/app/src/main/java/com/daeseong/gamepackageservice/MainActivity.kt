package com.daeseong.gamepackageservice

import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat

class MainActivity : AppCompatActivity() {

    private val tag = MainActivity::class.java.simpleName

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        Log.e(tag, "onCreate");

        //서비스 등록
        initService();

        //권한 체크
        checkPermissions();

        //프로그램 종료
        finish();
    }

    private fun initService() {

        try {

            if (GamePackageService.serviceIntent == null) {
                val service = Intent(this, GamePackageService::class.java)
                startService(service)
            }
        } catch (ex: Exception) {
            Log.e(tag, ex.message.toString())
        }
    }

    override fun onDestroy() {
        super.onDestroy()

        Log.d(tag, "onDestroy")
    }

    private fun checkPermissions() {

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {

            if (ContextCompat.checkSelfPermission(this, Manifest.permission.PACKAGE_USAGE_STATS) != PackageManager.PERMISSION_GRANTED ){

                val permissionList = arrayOf(Manifest.permission.PACKAGE_USAGE_STATS, Manifest.permission.INTERNET, Manifest.permission.ACCESS_NETWORK_STATE)
                ActivityCompat.requestPermissions(this, permissionList, 1)
            }
        }
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray) {

        when (requestCode) {
            1 -> {
                if (grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {

                    Log.e(tag, "PACKAGE_USAGE_STATS PERMISSION_GRANTED")
                } else {

                    Log.e(tag, "PACKAGE_USAGE_STATS PERMISSION_DENIED")
                }
            }
        }
    }

}
