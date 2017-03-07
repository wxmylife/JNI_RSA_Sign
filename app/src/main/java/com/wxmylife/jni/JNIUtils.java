package com.wxmylife.jni;

/**
 * Created by wxmylife on 2017/3/7.
 */

public class JNIUtils {

    static {
        System.loadLibrary("signUtil");
    }

    public static native String getPublicKey(Object obj);
}
