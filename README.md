##Android JNI获取应用签名

###一.前言：
>移动端数据加密，需要将RSA加密公钥存储，考虑到安全，现将公钥存在.so文件中，为了防止有人将.so文件拷贝使用，因此在里面验证app的签名，对比签名信息，如果正确，返回正确公钥。 

###二.参考：

[Android JNI NDK C++ so本地验证 获取应用签名
](http://www.jianshu.com/p/289c0b227902?utm_campaign=haruki&utm_content=note&utm_medium=reader_share&utm_source=qq)

[AndroidStudio 编译C/C++文件生成SO文件](http://blog.csdn.net/krubo1/article/details/50547681)

[android so 文件存私密数据，且防止 so文件未知应用盗用](http://blog.csdn.net/xx753277/article/details/37567951)

[Android JNI 获取应用签名](http://blog.csdn.net/masonblog/article/details/28095709)

[Android通过NDK获取Keystore签名值](http://blog.csdn.net/chenfeng0104/article/details/21641427)


###三.集成：
1. 配置ndk环境

	打开项目中Project Structure，配置ndk目录
	
	![示例图片](https://github.com/wxmylife/JNI_SignDemo/blob/master/img/1.png)

2. 配置app下的build.gradle

	![示例图片](https://github.com/wxmylife/JNI_SignDemo/blob/master/img/2.png)
	
	![示例图片](https://github.com/wxmylife/JNI_SignDemo/blob/master/img/3.png)
	
3. 修改项目下gradle.properties文件

	 填加如下命令:
	
	`android.useDeprecatedNdk=true`


4. 调用c++的java类

	**必须与app下的build.gradle中moduleName相同**
	
	```
	public class JNIUtils {
	
	    static {
	        System.loadLibrary("signUtil");//必须与app下的build.gradle中moduleName相同
	    }
	
	    public static native String getPublicKey(Object obj);
	}
	```

5. 生成class文件

	选择**Build**下的**Make Project**
	
	![示例图片](https://github.com/wxmylife/JNI_SignDemo/blob/master/img/4.png)

	![示例图片](https://github.com/wxmylife/JNI_SignDemo/blob/master/img/7.png)

6. 生成头文件

	进入Android Studio自带的终端中:
		 
	```cd app/src/main```
		 
	![示例图片](https://github.com/wxmylife/JNI_SignDemo/blob/master/img/9.png)
		
	通过如下命令生成头文件(Mac)：
		
	```javah -d jni -classpath /[项目地址]/app/build/intermediates/classes/debug [你的包名+包含native方法的类]```
		
	![示例图片](https://github.com/wxmylife/JNI_SignDemo/blob/master/img/10.png)
	
	![示例图片](https://github.com/wxmylife/JNI_SignDemo/blob/master/img/11.png)

7. 创建c++文件
	
	在jni文件夹下创建一个c++文件，名字可以取任意的
	
	![示例图片](https://github.com/wxmylife/JNI_SignDemo/blob/master/img/13.png)
	
	实现.h文件
	
	```
	
	#include <jni.h>
	#include <string.h>
	#include <stdio.h>
	#include "com_wxmylife_jni_JNIUtils.h" //导入.h头文件
	
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
	```
	
	8. java相关代码

		>获取App签名
		
		```
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
		```
	
	9. 完成效果
	
	|     成功    |    失败  | 
	|  :--------: | :--------:| 
	| ![示例图片](https://github.com/wxmylife/JNI_SignDemo/blob/master/gif/b.gif)  | ![示例图片](https://github.com/wxmylife/JNI_SignDemo/blob/master/gif/a.gif) | 
	
	
	
	
