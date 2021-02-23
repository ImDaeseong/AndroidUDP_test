package com.daeseong.sendudp

import android.os.AsyncTask
import android.util.Log
import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress


class UdpMessage(private val serverIP: String, private val port: Int, private val sMsg: String) : AsyncTask<Void?, Void?, Boolean>() {

    private val tag = javaClass.simpleName

    override fun doInBackground(vararg params: Void?): Boolean? {
        return sendMessage()
    }

    override fun onPostExecute(bResult: Boolean) {
        super.onPostExecute(bResult)
    }

    private fun sendMessage(): Boolean {

        try {

            val datagramSocket = DatagramSocket()
            datagramSocket.broadcast = true
            val address = InetAddress.getByName(serverIP)
            val bytesend: ByteArray = sMsg.toByteArray()
            val sendPacket = DatagramPacket(bytesend, bytesend.size, address, port)
            datagramSocket.send(sendPacket)
            datagramSocket.close()

            return true

        } catch (e: Exception) {
            Log.e(tag, e.message.toString())
        }

        return false
    }
}