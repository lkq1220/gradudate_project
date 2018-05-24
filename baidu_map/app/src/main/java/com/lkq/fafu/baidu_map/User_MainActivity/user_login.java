package com.lkq.fafu.baidu_map.User_MainActivity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.lkq.fafu.baidu_map.IP_get.GetIP;
import com.lkq.fafu.baidu_map.R;


/**
 * Created by alienware on 2018/3/26.
 */
public class user_login extends Activity {
    private Button button = null;
    private Button RegButton = null;
    private EditText mAccount;
    private EditText mPwd;


    class MyRegListener implements View.OnClickListener{
        @Override
        public void onClick(View v) {
            Intent RegIntent = new Intent(user_login.this,Register.class);
            startActivity(RegIntent);
        }
    }


    class MyLoginListener implements View.OnClickListener{
        @Override
        public void onClick(View v) {

            String name=mAccount.getText().toString();
            String pass=mPwd.getText().toString();
            Log.i("TAG", name + "_" + pass);
            UserDataManager uService=new UserDataManager(user_login.this);
            boolean flag=uService.login(name, pass);
            if(flag){
                Log.i("TAG", "登录成功");
                Intent Loginintent = new Intent(user_login.this,GetIP.class);
                startActivity(Loginintent);
            }else{
                Log.i("TAG","登录失败");
                Toast.makeText(user_login.this, "登录失败", Toast.LENGTH_LONG).show();
            }


            //Toast.makeText(getApplicationContext(), "pass success !", Toast.LENGTH_SHORT).show();//登录成功提示
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.user_login);
        button = (Button)findViewById(R.id.LoginButton);
        RegButton=(Button)findViewById(R.id.RegButton);
        mAccount = (EditText)findViewById(R.id.editnumber);
        mPwd = (EditText)findViewById(R.id.editpassword);

        button.setOnClickListener(new MyLoginListener());
        RegButton.setOnClickListener(new MyRegListener());
    }

}
