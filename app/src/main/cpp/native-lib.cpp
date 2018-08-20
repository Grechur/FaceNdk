#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <android/bitmap.h>
#include <android/log.h>
//使用命名空间
using namespace cv;



extern "C"{

JNIEXPORT void JNICALL
Java_com_example_zhouzhu_facendk_FaceDetection_loadCasecade(JNIEnv *env, jobject instance,
                                                            jstring path_);

JNIEXPORT jint JNICALL
Java_com_example_zhouzhu_facendk_FaceDetection_faceDetectionSaveInfo(JNIEnv *env, jobject instance,
                                                                     jobject bitmap);

void bitmap2Mat(JNIEnv *env, Mat &mat, jobject bitmap);

void mat2Bitmap(JNIEnv *env, Mat mat, jobject bitmap);
}

void bitmap2Mat(JNIEnv *env, Mat &mat, jobject bitmap) {
    AndroidBitmapInfo info;
    void* pixels;

    AndroidBitmap_getInfo(env,bitmap,&info);

    //锁定画布
    AndroidBitmap_lockPixels(env,bitmap,&pixels);

    //知道mat的宽高
    mat.create(info.height,info.width,CV_8UC4);
    if(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888){
        //对应的是CV_8UC4
        Mat temp(info.height,info.width,CV_8UC4,pixels);
        temp.copyTo(mat);
    } else if(info.format == ANDROID_BITMAP_FORMAT_RGB_565){
        //对应的是V_8UC2
        Mat temp(info.height,info.width,CV_8UC2,pixels);
        cvtColor(temp,mat,COLOR_BGR5652RGBA);
    }

    //解锁画布
    AndroidBitmap_unlockPixels(env,bitmap);
}

void mat2Bitmap(JNIEnv *env, Mat mat, jobject bitmap) {
    AndroidBitmapInfo info;
    void* pixels;
    //获得bitmap信息
    AndroidBitmap_getInfo(env,bitmap,&info);
    //锁定画布
    AndroidBitmap_lockPixels(env,bitmap,&pixels);


    if(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888){
        //对应的是CV_8UC4
        Mat temp(info.height,info.width,CV_8UC4,pixels);
        if(mat.type() == CV_8UC4){
            mat.copyTo(temp);
        }else if(mat.type() == CV_8UC2){
            cvtColor(mat,temp,COLOR_BGR5652BGRA);
        }else if(mat.type() == CV_8UC1){
            cvtColor(mat,temp,COLOR_GRAY2BGRA);
        }
    } else if(info.format == ANDROID_BITMAP_FORMAT_RGB_565){
        //对应的是V_8UC2
        Mat temp(info.height,info.width,CV_8UC2, pixels);
        if(mat.type() == CV_8UC4){
            cvtColor(mat,temp,COLOR_BGRA2BGR565);
        }else if(mat.type() == CV_8UC2){
            mat.copyTo(temp);
        }else if(mat.type() == CV_8UC1){//灰度
            cvtColor(mat,temp,COLOR_GRAY2BGR565);
        }
    }

    //解锁画布
    AndroidBitmap_unlockPixels(env,bitmap);
}



CascadeClassifier cascadeClassifier;
JNIEXPORT void JNICALL
Java_com_example_zhouzhu_facendk_FaceDetection_loadCasecade(JNIEnv *env, jobject instance,
                                                            jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);

    cascadeClassifier.load(path);
    env->ReleaseStringUTFChars(path_, path);
}

JNIEXPORT  jint JNICALL
Java_com_example_zhouzhu_facendk_FaceDetection_faceDetectionSaveInfo(JNIEnv *env, jobject instance,
                                                                     jobject bitmap) {
    Mat mat;
    //将bitmap转换为mat
    bitmap2Mat(env,mat,bitmap);

    //灰度化
    Mat gray_mat;
    cvtColor(mat,gray_mat,COLOR_BGRA2GRAY);

    //直方均衡补偿
    Mat equalize_mat;
    equalizeHist(gray_mat,equalize_mat);

    // 4. 检测人脸，这是个大问题
    std::vector<Rect> faces;
    cascadeClassifier.detectMultiScale(equalize_mat, faces, 1.1, 5, 0 | CV_HAAR_SCALE_IMAGE,
                                       Size(160, 160));

//    LOGE("检测到人脸的个数：%d", faces.size());

//    cascadeClassifier.getOldCascade();
    if(faces.size() == 1){
        Rect faceRect = faces[0];

        //把人脸圈出来
        rectangle(mat,faceRect,Scalar(255,155,155),20);

        //保存人脸
//        imwrite();
    }

    //将mat转换成bitmap
    mat2Bitmap(env,mat,bitmap);
    return 0;
}

