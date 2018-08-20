package com.example.zhouzhu.facendk;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
    TextView tv;
    ImageView image;
    private Bitmap mFaceBitmap;
    FaceDetection mFaceDetection;
    private File mCascadeFile;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        tv = (TextView) findViewById(R.id.sample_text);
        image = findViewById(R.id.image);
        mFaceBitmap = BitmapFactory.decodeResource(getResources(),R.mipmap.meinv);
        mFaceDetection = new FaceDetection();

        copyCasecadeFile();

        mFaceDetection.loadCasecade(mCascadeFile.getAbsolutePath());

        image.setImageBitmap(mFaceBitmap);
        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mFaceDetection.faceDetectionSaveInfo(mFaceBitmap);
                image.setImageBitmap(mFaceBitmap);
            }
        });
    }

    private void copyCasecadeFile() {
        try {
            // load cascade file from application resources
            InputStream is = getResources().openRawResource(R.raw.lbpcascade_frontalface);
            File cascadeDir = getDir("cascade", Context.MODE_PRIVATE);
            mCascadeFile = new File(cascadeDir, "lbpcascade_frontalface.xml");
            if(mCascadeFile.exists()) return;
            FileOutputStream os = new FileOutputStream(mCascadeFile);

            byte[] buffer = new byte[4096];
            int bytesRead;
            while ((bytesRead = is.read(buffer)) != -1) {
                os.write(buffer, 0, bytesRead);
            }
            is.close();
            os.close();
        }catch (Exception e){
            
        }
    }

}
