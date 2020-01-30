package com.daeseong.sendudp;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

public class MainActivity extends AppCompatActivity {

    private final String TAG = getClass().getSimpleName();

    private Button btnSend;

    private String sLocalIP = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        setLocalIP();

        btnSend = (Button)findViewById(R.id.btnSend);
        btnSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                try {

                    setLocalIP();

                    if(sLocalIP == null) {
                        sLocalIP = "255.255.255.255";
                    }

                    UdpMessage udpMessage = new UdpMessage(sLocalIP, 80, "s:packagename:10초");
                    //UdpMessage udpMessage = new UdpMessage(sLocalIP, 80, "u:packagename:20초");
                    //UdpMessage udpMessage = new UdpMessage(sLocalIP, 80, "e:packagename:30초");
                    udpMessage.execute();

                }catch (Exception ex){
                    Log.e(TAG, ex.getMessage().toString());
                }

            }
        });

    }

    private void setLocalIP(){

        try {
            String sIP = getLocalIpAddress();
            String[] sArray = sIP.split("\\.");
            sLocalIP = sArray[0] + "." + sArray[1] + "." + sArray[2] + ".2";

            Log.e(TAG, "sLocalIP: " + sLocalIP);

        }catch (Exception ex){
            Log.e(TAG, ex.getMessage().toString());
        }
    }

    private static String getLocalIpAddress() {
        try {
            for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements();) {
                NetworkInterface intf = en.nextElement();
                for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements();) {
                    InetAddress inetAddress = enumIpAddr.nextElement();
                    if (!inetAddress.isLoopbackAddress() && inetAddress instanceof Inet4Address) {
                        return inetAddress.getHostAddress();
                    }
                }
            }
        } catch (SocketException ex) {
            ex.printStackTrace();
        }
        return null;
    }

}
