package com.lkq.fafu.baidu_map.User_MainActivity;

import java.io.Serializable;

public class UserData implements Serializable{
    private int id;
    private String username;
    private String password;
    private int age;
    private String sex;
    public UserData() {
        super();
        // TODO Auto-generated constructor stub
    }
    public UserData(String username, String password, int age, String sex) {
        super();
        this.username = username;
        this.password = password;
        this.age = age;
        this.sex = sex;
    }
    public int getId() {
        return id;
    }
    public void setId(int id) {
        this.id = id;
    }
    public String getUsername() {
        return username;
    }
    public void setUsername(String username) {
        this.username = username;
    }
    public String getPassword() {
        return password;
    }
    public void setPassword(String password) {
        this.password = password;
    }
    @Override
    public String toString() {
        return "User [id=" + id + ", username=" + username + ", password="
                + password + "]";
    }

}

