package com.daeseong.gamepackageservice

import android.annotation.SuppressLint
import android.annotation.TargetApi
import android.app.Service
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.net.ConnectivityManager
import android.os.Build
import android.os.Handler
import android.os.IBinder
import android.util.Log
import android.widget.Toast
import org.json.JSONObject
import java.net.Inet4Address
import java.net.NetworkInterface
import java.net.SocketException
import java.text.SimpleDateFormat
import java.util.*
import kotlin.collections.HashMap


class GamePackageService : Service() {

    private val tag = GamePackageService::class.java.simpleName

    companion object {
        var serviceIntent: Intent? = null
    }

    private var timerTask: TimerTask? = null
    private var timer: Timer? = null

    private val gameinfoMap: HashMap<String, gameinfo> = HashMap()

    private var sLocalIP: String? = null
    private val nPort = 80

    override fun onCreate() {
        super.onCreate()

        initData()
        setLocalIP()
    }

    @TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {

        Log.d(tag, "onStartCommand")

        serviceIntent = intent
        startTimer()

        return START_STICKY
    }

    override fun onDestroy() {
        super.onDestroy()

        serviceIntent = null

        Log.d(tag, "onDestroy")

        closeTimer()
        handler.removeMessages(0)
    }

    override fun onBind(intent: Intent?): IBinder? {
        return null
    }

    @SuppressLint("NewApi")
    fun startTimer() {

        try {

            timerTask = object : TimerTask() {
                override fun run() {
                    GetAllPackageNames()
                }
            }
            timer = Timer()
            timer!!.schedule(timerTask, 0, 10000)

        } catch (ex: Exception) {
            Log.e(tag, "startTimer:" + ex.message.toString())
        }
    }

    //전체앱 목록
    private fun GetAllPackageNames() {

        try {

            val packageManager: PackageManager = packageManager
            for (applicationInfo in packageManager.getInstalledApplications(PackageManager.GET_META_DATA)) {

                if (applicationInfo.packageName == "com.daeseong.gamepackageservice") {
                    continue
                }

                if (gameinfoMap.containsKey(applicationInfo.packageName)) {
                    continue
                }

                //항목에 있는 패키지명 등록
                if (iteminfo.getInstance().isGameItem(applicationInfo.packageName)) {
                    gameinfoMap[applicationInfo.packageName] = gameinfo(applicationInfo.packageName)
                }
            }

            val result = JSONObject()
            /*
            var i = 1
            for (key in gameinfoMap.keys) {
                result.put(i.toString(), gameinfoMap[key]!!.packagename)
                i++
            }
            */
            for (key in gameinfoMap.keys) {
                result.put(key, gameinfoMap[key]!!.packagename)
            }
            gameinfoMap.clear()
            SendMessage(result.toString())

        } catch (e: Exception) {
        }
    }

    private fun closeTimer() {

        try {

            if (timerTask != null) {
                timerTask!!.cancel()
                timerTask = null
            }

            if (timer != null) {
                timer!!.cancel()
                timer!!.purge()
                timer = null
            }

        } catch (ex: Exception) {
            Log.e(tag, "stopTimer:" + ex.message.toString())
        }
    }

    private val handler = Handler { msg ->
        try {
            Toast.makeText(applicationContext, msg.obj.toString(), Toast.LENGTH_SHORT).show()
        } catch (ex: java.lang.Exception) {
            Log.e(tag, "Toast" + ex.message.toString())
        }
        true
    }

    private fun initData() {

        Log.e(tag, "initData")

        try {

            if (isNetworkAvailable(this)) {
                val downloadJson = DownloadJson()
                downloadJson.execute()
            }

        } catch (ex: Exception) {
            Log.e(tag, ex.message.toString())
        }

        //sample
        iteminfo.getInstance().setGameItem("com.kakaogames.moonlight")
        iteminfo.getInstance().setGameItem("com.google.android.youtube")
    }

    private fun getTimeDate(): String? {
        val dateFormat = SimpleDateFormat("HH:mm:ss")
        return dateFormat.format(Date())
    }

    private fun getTimeDate(date: Long): String? {
        val dateFormat = SimpleDateFormat("HH:mm:ss")
        return dateFormat.format(Date(date))
    }

    private fun isNetworkAvailable(context: Context): Boolean {
        val connectivityManager = context.getSystemService(CONNECTIVITY_SERVICE) as ConnectivityManager
        val networkInfo = connectivityManager.activeNetworkInfo
        return networkInfo != null && networkInfo.isConnected
    }

    private fun SendMessage(sMsg: String) {

        try {

            if (sLocalIP == null) {
                sLocalIP = "255.255.255.255"
            }

            val udpMessage = UdpMessage(sLocalIP!!, nPort, sMsg)
            udpMessage.execute()
        } catch (ex: Exception) {
            Log.e(tag, ex.message.toString())
        }
    }

    private fun setLocalIP() {

        try {

            val sIP = getLocalIpAddress()
            val sArray = sIP!!.split("\\.").toTypedArray()
            sLocalIP = sArray[0] + "." + sArray[1] + "." + sArray[2] + ".2"
            Log.e(tag, "sLocalIP: $sLocalIP")
        } catch (ex: java.lang.Exception) {
            Log.e(tag, ex.message.toString())
        }
    }

    private fun getLocalIpAddress(): String? {

        try {

            val en = NetworkInterface.getNetworkInterfaces()
            while (en.hasMoreElements()) {
                val intf = en.nextElement()
                val enumIpAddr = intf.inetAddresses
                while (enumIpAddr.hasMoreElements()) {
                    val inetAddress = enumIpAddr.nextElement()
                    if (!inetAddress.isLoopbackAddress && inetAddress is Inet4Address) {
                        return inetAddress.getHostAddress()
                    }
                }
            }
        } catch (ex: SocketException) {
            ex.printStackTrace()
        }
        return null
    }

    private fun showMessage(sMsg: String) {

        try {
            val msg = handler.obtainMessage()
            msg.what = 0
            msg.obj = sMsg
            handler.sendMessage(msg)
        } catch (ex: java.lang.Exception) {
            Log.e(tag, "Toast" + ex.message.toString())
        }
    }
}