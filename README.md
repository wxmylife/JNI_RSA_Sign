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
1.配置ndk环境
打开项目中Project Structure，配置ndk目录(如何下载，自行描述)

[示例图片](https://github.com/wxmylife/JNI_SignDemo/blob/master/img/1.png)

2.配置app下的build.gradle

[示例图片](https://github.com/wxmylife/JNI_SignDemo/blob/master/img/2.png)

3.修改项目下gradle.properties文件

填加如下命令

`android.useDeprecatedNdk=true`


3.调用c++的java类

**必须与app下的build.gradle中moduleName相同**

```
public class JNIUtils {

    static {
        System.loadLibrary("signUtil");//必须与app下的build.gradle中moduleName相同
    }

    public static native String getPublicKey(Object obj);
}
```
4.生成class文件

选择**Build**下的**Make Project**

[示例图片](https://github.com/wxmylife/JNI_SignDemo/blob/master/img/4.png)

生成的class文件：

[示例图片](https://github.com/wxmylife/JNI_SignDemo/blob/master/img/7.png)

5.生成头文件