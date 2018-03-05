

package com.imageEditor.android;

import org.qtproject.qt5.android.bindings.QtApplication;
import org.qtproject.qt5.android.bindings.QtActivity;

import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MenuItem.OnMenuItemClickListener;
import android.widget.ImageView;
import android.widget.Toast;
import android.os.Bundle;
import android.os.Environment;
import android.app.ActionBar;
import android.app.Activity;
import android.provider.MediaStore;
import android.util.Log;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import java.io.FileOutputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
public class PhotoArt extends QtActivity
{
    public static native void fileSelected(String fileName);

    static final int REQUEST_OPEN_IMAGE = 1;

    private static PhotoArt m_instance;

    public PhotoArt()
    {
        m_instance = this;
    }

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
    }

    static void openAnImage()
    {
        m_instance.dispatchOpenGallery();
    }
//////////////////////////////////////
    static File folder;
    static void createDirIfNotExists() {
       folder = new File(Environment.getExternalStorageDirectory() +
                                     File.separator + "PhotoArt");
      //System.out.println(Environment.getExternalStorageDirectory() +File.separator + "PhotoArt");
        boolean success = true;
        if (!folder.exists()) {
            success = folder.mkdirs();
        }
        if (success) {
            // Do something on success
        } else {
            // Do something else on failure
        }
}

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
       if (resultCode == RESULT_OK)
        {
            if(requestCode == REQUEST_OPEN_IMAGE)
            {
                String filePath = getRealPathFromURI(getApplicationContext(), data.getData());
                fileSelected(filePath);
            }
        }
        else
        {
            fileSelected(":(");
        }

        super.onActivityResult(requestCode, resultCode, data);
    }

    private void dispatchOpenGallery()
    {
        Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
        intent.setType("image/*");
        startActivityForResult(intent, REQUEST_OPEN_IMAGE);
    }

    public String getRealPathFromURI(Context context, Uri contentUri)
    {
        Cursor cursor = null;
        try
        {
            String[] proj = { MediaStore.Images.Media.DATA };
            cursor = context.getContentResolver().query(contentUri,  proj, null, null, null);
            int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
            cursor.moveToFirst();
            return cursor.getString(column_index);
        }
        finally
        {
            if (cursor != null)
            {
                cursor.close();
            }
        }
    }

}

