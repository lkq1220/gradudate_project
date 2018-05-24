package com.lkq.fafu.baidu_map;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;

import com.lkq.fafu.baidu_map.User_MainActivity.user_login;


public class HelloActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_hello);
        Intent mainIntent = new Intent(HelloActivity.this, user_login.class);
        new Handler().postDelayed(new Runnable() {
            public void run() {
                /* Create an Intent that will start the Main WordPress Activity. */
                Intent mainIntent = new Intent(HelloActivity.this, user_login.class);
                HelloActivity.this.startActivity(mainIntent);
                HelloActivity.this.finish();
            }
        }, 3000);

    }
}
