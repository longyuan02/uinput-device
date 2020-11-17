package com.action.androidq;

import android.content.Context;
import android.os.Environment;
import android.util.Log;
import android.util.Xml;

import org.xmlpull.v1.XmlSerializer;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

public class SecondActivity {
    private static final String TAG = SecondActivity.class.getSimpleName();
    private Context context;

    public SecondActivity(Context context) {
        this.context = context;
        method();
        Log.e(TAG, "test");
    }

    private void method() {
        // 模拟20条Person数据
        List<Person> persons = new ArrayList<Person>();
        for (int i = 0; i < 20; i++) {
            persons.add(new Person(i, "zhangsan" + i, i + 10));
        }
        Log.d(TAG, "persons.size():" + persons.size()); // persons.size():20

        try {
            // 如何创建xml文件，然后把这20条数据，写入xml文件里面去，一定是去看Android提供的API
            XmlSerializer serializer = Xml.newSerializer();
            // 指定流目录
            OutputStream os = openFileOutput("persons.xml", Context.MODE_PRIVATE);
            // 设置指定目录
            serializer.setOutput(os, "UTF-8");

            // 开始写入Xml格式数据
            // 开始文档
            // 参数一：指定编码格式   参数二：是不是独立的xml(这个xml与其他xml是否有关联)
            serializer.startDocument("UTF-8", true);

            // 开始根标签
            // 参数一：命名空间   参数二：标签名称
            serializer.startTag(null, "persons");

            for (Person person : persons) {
                // 开始子标签
                serializer.startTag(null, "person");

                // 设置属性
                serializer.attribute(null, "id", String.valueOf(person.getId()));

                // 设置name
                // 开始name标签
                serializer.startTag(null, "name");
                // 写入值
                serializer.text(person.getName());
                // 结束name标签
                serializer.endTag(null, "name");

                // 设置age
                // 开始age标签
                serializer.startTag(null, "age");
                // 写入age值
                serializer.text(String.valueOf(person.getAge()));
                // 结束age标签
                serializer.endTag(null, "age");

                // 结束子标签
                serializer.endTag(null, "person");
            }

            // 结束根标签
            serializer.endTag(null, "persons");

            // 结束文档
            serializer.endDocument();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private OutputStream openFileOutput(String filePath, int permission) {
        OutputStream outputStream = null;
        try {//getExternalFilesDir(Environment.DIRECTORY_MOVIES);
            outputStream = new FileOutputStream(new File(context.getExternalFilesDir(Environment.DIRECTORY_DOCUMENTS).getAbsolutePath()) + "/"+filePath);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            Log.e(TAG, "erroe" + e.getMessage().toString());
        } finally {
            return outputStream;
        }

    }
}
