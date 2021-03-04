package com.daeseong.gamepackageservice;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Build;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

import org.json.JSONObject;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Timer;
import java.util.TimerTask;

public class GamePackageService extends Service {

    private static final String TAG = GamePackageService.class.getSimpleName();

    public static Intent serviceIntent = null;

    private TimerTask timerTask = null;
    private Timer timer = null;

    private HashMap<String, gameinfo> gameinfoMap = new HashMap<>();

    private String sLocalIP = null;
    private int nPort = 80;

    @Override
    public void onCreate() {

        super.onCreate();

        initData();

        setLocalIP();
    }

    @TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {

        Log.d(TAG, "onStartCommand");

        serviceIntent = intent;

        startTimer();

        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        serviceIntent = null;

        Log.d(TAG, "onDestroy");

        closeTimer();

        handler.removeMessages(0);
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @SuppressLint("NewApi")
    public void startTimer(){

        try {

            timerTask = new TimerTask() {
                @Override
                public void run() {

                    GetAllPackageNames();
                }
            };

            timer = new Timer();
            timer.schedule(timerTask, 0, 10000);

        }catch (Exception ex){
            Log.e(TAG, "startTimer:" + ex.getMessage().toString());
        }
    }

    //전체앱 목록
    private void GetAllPackageNames(){

        try
        {
            PackageManager packageManager = getPackageManager();

            for(ApplicationInfo applicationInfo : packageManager.getInstalledApplications(PackageManager.GET_META_DATA)){

                if(applicationInfo.packageName.equals("com.daeseong.gamepackageservice") ){
                    continue;
                }

                if ( gameinfoMap.containsKey(applicationInfo.packageName) ){
                    continue;
                }

                //항목에 있는 패키지명 등록
                if (iteminfo.getInstance().isGameItem(applicationInfo.packageName) ){
                    gameinfoMap.put(applicationInfo.packageName, new gameinfo(applicationInfo.packageName));
                }
            }

            JSONObject result = new JSONObject();
            /*
            Integer i =1;
            for(String key : gameinfoMap.keySet()){
                result.put(String.valueOf(i), gameinfoMap.get(key).getPackagename());
                i++;
            }
            */
            for(String key : gameinfoMap.keySet()){
                result.put(key, gameinfoMap.get(key).getPackagename());
            }
            gameinfoMap.clear();
            SendMessage(result.toString());
        }
        catch(Exception e) {
        }
    }

    private void closeTimer(){

        try {

            if (timerTask != null) {
                timerTask.cancel();
                timerTask = null;
            }

            if (timer != null) {
                timer.cancel();
                timer.purge();
                timer = null;
            }

        }catch (Exception ex){
            Log.e(TAG, "stopTimer:" + ex.getMessage().toString());
        }
    }

    private Handler handler = new Handler(new Handler.Callback() {
        @Override
        public boolean handleMessage(Message msg) {

            try {
                Toast.makeText(getApplicationContext(), msg.obj.toString(), Toast.LENGTH_SHORT).show();
            }catch (Exception ex){
                Log.e(TAG, "Toast" + ex.getMessage().toString());
            }
            return true;
        }
    });

    private void initData(){

        Log.e(TAG, "initData");

        try {

            if (isNetworkAvailable(this)) {
                DownloadJson downloadJson = new DownloadJson();
                downloadJson.execute();
            }
        }catch (Exception ex){
            Log.e(TAG, ex.getMessage().toString());
        }

        //sample
        iteminfo.getInstance().setGameItem("com.kakaogames.moonlight");
        iteminfo.getInstance().setGameItem("com.google.android.youtube");
    }

    private static String getTimeDate() {
        SimpleDateFormat dateFormat = new SimpleDateFormat("HH:mm:ss");
        return dateFormat.format(new Date());
    }

    private static String getTimeDate(long date) {
        SimpleDateFormat dateFormat = new SimpleDateFormat("HH:mm:ss");
        return dateFormat.format(new Date(date));
    }

    private static boolean isNetworkAvailable(Context context)
    {
        ConnectivityManager connectivityManager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo = connectivityManager.getActiveNetworkInfo();
        if (networkInfo != null && networkInfo.isConnected()){
            return true;
        }
        return false;
    }

    private void SendMessage(String sMsg){

        try {

            if(sLocalIP == null) {
                sLocalIP = "255.255.255.255";
            }
            UdpMessage udpMessage = new UdpMessage(sLocalIP, nPort, sMsg);
            udpMessage.execute();
        }catch (Exception ex){
            Log.e(TAG, ex.getMessage().toString());
        }
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

    private void showMessage(String sMsg){

        try {
            Message msg = handler.obtainMessage();
            msg.what = 0;
            msg.obj = sMsg;
            handler.sendMessage(msg);
        }catch (Exception ex){
            Log.e(TAG, "Toast" + ex.getMessage().toString());
        }
    }
}
