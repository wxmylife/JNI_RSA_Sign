//
// Created by wxmylife on 2017/3/7.
//

#include <jni.h>
#include <string.h>
#include <stdio.h>
#include "com_wxmylife_jni_JNIUtils.h"

/**
 *这个key是和服务器之间通信的秘钥
 */
const char* AUTH_KEY = "keyValue";

/**
 * 发布的app 签名,只有和本签名一致的app 才会返回 AUTH_KEY
 * 这个RELEASE_SIGN的值是上一步用java代码获取的值
 */
const char* RELEASE_SIGN = "308201dd30820146020101300d06092a864886f70d010105050030373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b3009060355040613025553301e170d3136313132353137343333315a170d3436313131383137343333315a30373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b300906035504061302555330819f300d06092a864886f70d010101050003818d0030818902818100c0f7cb6d1cd3af254945c8e784545666ac707b31f754b01a772817ece6e64b58c834ff08cb142b9cf2b1f73ee324c2efa199d5aecb7453573161a538d031c59914af4bb857b3fa36d0a46dc1f03fd130df02f17b67f4731f7333c15035866cbc7a997c2deb90336f39e7191a5826311428de7b1ba8fd948a0aa457223281c0df0203010001300d06092a864886f70d0101050500038181004d8dd7463108247341244ebc4973aaf93b33f8cf9703c4d9be80a368b6f47701f66ece6cb1121014dc1c5b42c49eae86b2a4620286aba1b221ece7fd23dc7329c8486c66a51ab8f127c9adb86944957b159b7b4a5e30b716f48524d0c2c75608532c747c70aca8ffdcaa2bbdb00f6f62f37b939b377460e1a7560101d1280a65";

/**
 * 发布的app 签名 的HashCode
 */
const int RELEASE_SIGN_HASHCODE = -332752192;

JNIEXPORT jstring JNICALL Java_com_wxmylife_jni_JNIUtils_getPublicKey
  (JNIEnv *env, jclass jclazz, jobject contextObject){

    jclass native_class = env->GetObjectClass(contextObject);
    jmethodID pm_id = env->GetMethodID(native_class, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jobject pm_obj = env->CallObjectMethod(contextObject, pm_id);
    jclass pm_clazz = env->GetObjectClass(pm_obj);
    // 得到 getPackageInfo 方法的 ID
    jmethodID package_info_id = env->GetMethodID(pm_clazz, "getPackageInfo","(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jclass native_classs = env->GetObjectClass(contextObject);
    jmethodID mId = env->GetMethodID(native_classs, "getPackageName", "()Ljava/lang/String;");
    jstring pkg_str = static_cast<jstring>(env->CallObjectMethod(contextObject, mId));
    // 获得应用包的信息
    jobject pi_obj = env->CallObjectMethod(pm_obj, package_info_id, pkg_str, 64);
    // 获得 PackageInfo 类
    jclass pi_clazz = env->GetObjectClass(pi_obj);
    // 获得签名数组属性的 ID
    jfieldID signatures_fieldId = env->GetFieldID(pi_clazz, "signatures", "[Landroid/content/pm/Signature;");
    jobject signatures_obj = env->GetObjectField(pi_obj, signatures_fieldId);
    jobjectArray signaturesArray = (jobjectArray)signatures_obj;
    jsize size = env->GetArrayLength(signaturesArray);
    jobject signature_obj = env->GetObjectArrayElement(signaturesArray, 0);
    jclass signature_clazz = env->GetObjectClass(signature_obj);

    //第一种方式--检查签名字符串的方式
    jmethodID string_id = env->GetMethodID(signature_clazz, "toCharsString", "()Ljava/lang/String;");
    jstring str = static_cast<jstring>(env->CallObjectMethod(signature_obj, string_id));
    char *c_msg = (char*)env->GetStringUTFChars(str,0);

    if(strcmp(c_msg,RELEASE_SIGN)==0)//签名一致  返回合法的 api key，否则返回错误
    {
        return (env)->NewStringUTF(AUTH_KEY);
    }else
    {
        return (env)->NewStringUTF("error");
    }

    //第二种方式--检查签名的hashCode的方式
    /*
    jmethodID int_hashcode = env->GetMethodID(signature_clazz, "hashCode", "()I");
    jint hashCode = env->CallIntMethod(signature_obj, int_hashcode);
    if(hashCode == RELEASE_SIGN_HASHCODE)
    {
        return (env)->NewStringUTF(AUTH_KEY);
    }else{
        return (env)->NewStringUTF("错误");
    }
     */
}