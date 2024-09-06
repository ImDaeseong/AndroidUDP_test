package com.daeseong.sendudp

import android.os.Bundle
import android.util.Log
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import java.net.Inet4Address
import java.net.NetworkInterface
import java.net.SocketException

class MainActivity : AppCompatActivity() {

    private val tag = javaClass.simpleName

    private var btnSend: Button? = null

    private var sLocalIP: String? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        setLocalIP()

        btnSend = findViewById<Button>(R.id.btnSend)
        btnSend!!.setOnClickListener {

            try {

                setLocalIP()

                if (sLocalIP == null) {
                    sLocalIP = "255.255.255.255"
                }

                val udpMessage = UdpMessage(sLocalIP!!, 80, "s:packagename:10초")
                //val udpMessage = UdpMessage(sLocalIP!!, 80, "u:packagename:20초")
                //val udpMessage = UdpMessage(sLocalIP!!, 80, "e:packagename:30초")
                udpMessage.execute()

            } catch (ex: java.lang.Exception) {
                Log.e(tag, ex.message.toString())
            }

        }
    }

    private fun setLocalIP() {

        try {

            val sIP = getLocalIpAddress()

            //Log.e(tag, "sIP: $sIP")
            /*
            val sArray = sIP!!.split('.')
            for (sIP in sArray){
                Log.e(tag, "sIP: $sIP")
            }
            */

            val sArray = sIP!!.split('.').toTypedArray()
            //Log.e(tag, "sArray0: ${sArray[0]}")
            //Log.e(tag, "sArray1: ${sArray[1]}")
            //Log.e(tag, "sArray2: ${sArray[2]}")
            //Log.e(tag, "sArray3: ${sArray[3]}")

            sLocalIP = sArray[0] + "." + sArray[1] + "." + sArray[2] + ".2"
            //Log.e(tag, "sLocalIP: $sLocalIP")

        } catch (ex: Exception) {

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

}
