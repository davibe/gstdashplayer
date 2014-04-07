//package com.cedeo.DashPlayer
public class DashPlayer {

    static {
        System.load("/Users/dade/work/gstdashplayer/.libs/libdashplayer.dylib");
        
    }

    public static native void dash_player_jni_initialize();

    public static native void dash_player_play();

    public static void main(String[] argv) {
      DashPlayer.dash_player_jni_initialize();
      DashPlayer.dash_player_play();
      while(true){
        int i = 1;
      }
    }
}
