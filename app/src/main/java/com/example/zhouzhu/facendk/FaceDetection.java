package com.example.zhouzhu.facendk;

import android.graphics.Bitmap;

/**
 * Created by zhouzhu on 2018/7/29.
 */

public class FaceDetection {
    static {
        System.loadLibrary("native-lib");
    }
    public native int faceDetectionSaveInfo(Bitmap bitmap);

    /**
     * 加载人脸识别的分类器
     * @param path
     * @return
     */
    public native void loadCasecade(String path);
}
