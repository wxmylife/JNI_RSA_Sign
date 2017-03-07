package com.wxmylife.jni;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = MainActivity.class.getSimpleName();

    private String publicKey;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findViewById(R.id.btn_sign).setOnClickListener(new View.OnClickListener() {
            @Override public void onClick(View view) {
                publicKey=  JNIUtils.getPublicKey(MainActivity.this);
                ((TextView)findViewById(R.id.txt_key)).setText(publicKey);
            }
        });



        // String debugSign=getSignature(this);
        // if (!TextUtils.isEmpty(debugSign)){
        //     Log.e(TAG,"debug签名为------>>>"+debugSign);
        // }

    }


    public static String getSignature(Context context)
    {
        try {
            /** 通过包管理器获得指定包名包含签名的包信息 **/
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_SIGNATURES);
            /******* 通过返回的包信息获得签名数组 *******/
            Signature[] signatures = packageInfo.signatures;
            /******* 循环遍历签名数组拼接应用签名 *******/
            return signatures[0].toCharsString();
            /************** 得到应用签名 **************/
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return null;
    }
}
