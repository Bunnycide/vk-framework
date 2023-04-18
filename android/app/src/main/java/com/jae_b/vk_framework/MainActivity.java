package com.jae_b.vk_framework;

import android.app.NativeActivity;

public class MainActivity extends NativeActivity {
    static {
        System.loadLibrary("vk_main");
    }
}
