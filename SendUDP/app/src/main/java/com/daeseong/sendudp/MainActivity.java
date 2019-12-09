package com.daeseong.sendudp;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    private final String TAG = getClass().getSimpleName();

    private Button btnSend;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btnSend = (Button)findViewById(R.id.btnSend);
        btnSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                try {
                    UdpMessage udpMessage = new UdpMessage("255.255.255.255", 20121, "test message test message test message test message test message");
                    udpMessage.execute();
                }catch (Exception ex){
                    Log.e(TAG, ex.getMessage().toString());
                }

            }
        });

    }

}
