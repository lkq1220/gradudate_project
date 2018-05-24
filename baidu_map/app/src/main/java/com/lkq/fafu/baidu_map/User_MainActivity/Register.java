package com.lkq.fafu.baidu_map.User_MainActivity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.lkq.fafu.baidu_map.R;


public class Register extends Activity {
    EditText username;
    EditText password;
    EditText checkwd;
    Button register;
    Button cancel;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_register);
        findViews();
        register.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                String name = username.getText().toString().trim();
                String pass = password.getText().toString().trim();
                String checkpass = checkwd.getText().toString().trim();

                if(pass.equals(checkpass)==false){
                    Toast.makeText(Register.this, "密码不一致，重新输入", Toast.LENGTH_LONG).show();
                    return ;
                }

                Log.i("TAG", name + "_" + pass);

                UserDataManager uService = new UserDataManager(Register.this);
                UserData user = new UserData();
                user.setUsername(name);
                user.setPassword(pass);
                uService.register(user);
                Toast.makeText(Register.this, "注册成功", Toast.LENGTH_LONG).show();
                Intent intentcancel = new Intent(Register.this, user_login.class);
                startActivity(intentcancel);
                finish();
            }
        });
        cancel.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intentcancel = new Intent(Register.this, user_login.class);
                startActivity(intentcancel);
                Toast.makeText(Register.this, "注册失败", Toast.LENGTH_LONG).show();
                finish();
            }
        });
    }
    private void findViews() {
        username=(EditText) findViewById(R.id.resetpwd_edit_name);
        password=(EditText) findViewById(R.id.resetpwd_edit_pwd_old);
        checkwd =(EditText) findViewById(R.id.resetpwd_edit_pwd_new);
        register=(Button) findViewById(R.id.register_btn_sure);
        cancel  =(Button) findViewById(R.id.register_btn_cancel);

    }

}
