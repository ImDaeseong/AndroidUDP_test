package com.daeseong.sendudp;

import android.os.AsyncTask;
import android.util.Log;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class UdpMessage extends AsyncTask<Void, Void, Boolean> {

    private final String TAG = getClass().getSimpleName();

    private String serverIP;
    private int port;
    private String sMsg;

    public UdpMessage(String serverIP, int port, String sMsg)
    {
        this.serverIP = serverIP;
        this.port = port;
        this.sMsg = sMsg;
    }

    @Override
    protected Boolean doInBackground(Void... voids) {
        return sendMessage();
    }

    @Override
    protected void onPostExecute(Boolean bResult) {
        super.onPostExecute(bResult);
    }

    private boolean sendMessage() {
        try {
            DatagramSocket datagramSocket = new DatagramSocket();
            datagramSocket.setBroadcast(true);
            InetAddress address = InetAddress.getByName(serverIP);

            byte[] bytesend;
            bytesend = sMsg.getBytes();
            DatagramPacket sendPacket = new DatagramPacket(bytesend, bytesend.length, address, port);
            datagramSocket.send(sendPacket);
            datagramSocket.close();
            return true;

        } catch (Exception e) {
            Log.e(TAG, e.getMessage().toString());
        }
        return false;
    }
}

