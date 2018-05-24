package com.lkq.fafu.baidu_map.DeepAI;

import android.content.res.AssetManager;
import android.os.Trace;

import org.tensorflow.contrib.android.TensorFlowInferenceInterface;


public class MyTSF {

    static {
        //加载库文件
        System.loadLibrary("tensorflow_inference");
    }

    private static final String MODEL_FILE = "file:///android_asset/lkq.pb"; //模型存放路径

    //数据的维度
    private static final int HEIGHT = 5;
    private static final int WIDTH = 1;

    //模型中输出变量的名称
    private static final String inputName = "lkq_input";


    //模型中输出变量的名称
    private static final String outputName = "lkq_output";
    //样本数量
    private static final int NUM_CLASSES=3;

    //用于存储的模型输入数据
    private float[] inputs = new float[HEIGHT * WIDTH];
    //用于存储模型的输出数据
    private float[] outputs = new float[NUM_CLASSES];



    TensorFlowInferenceInterface inferenceInterface;


    public MyTSF(AssetManager assetManager, float[] input_data) {
        //接口定义
        this.inputs=input_data;
        inferenceInterface = new TensorFlowInferenceInterface(assetManager,MODEL_FILE);
    }

    public float[] getAddResult() {
        //为输入数据赋值
//        inputs[0]=5.1f;
//        inputs[1]=3.5f;
//        inputs[2]=1.4f;
//        inputs[3]=0.2f;
        //将数据feed给tensorflow
        Trace.beginSection("feed");
        inferenceInterface.feed(inputName, inputs, WIDTH, HEIGHT);
        Trace.endSection();

        //运行
        Trace.beginSection("run");
        String[] outputNames = new String[] {outputName};
        inferenceInterface.run(outputNames);
        Trace.endSection();

        //将输出存放到outputs中
        Trace.beginSection("fetch");
        inferenceInterface.fetch(outputName, outputs);
        Trace.endSection();

        return outputs;
    }


}
