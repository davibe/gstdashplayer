//package com.cedeo.DashPlayer
public class DashPlayer {

    static {
      // this hack is done because .loadLibrary seems not to work correctly
      // on macosx (mavericks at least)
      String path = System.getProperty("java.library.path");
      System.load(path + "/.libs/libdashplayer.dylib");
    }
    
    public long reference;

    public DashPlayer() {
      this.reference = DashPlayer.dash_player_jni_initialize();
    }

    public void setUri(String uri) {
      DashPlayer.dash_player_set_uri(this.reference, uri);
    }

    public void play() {
      DashPlayer.dash_player_play(this.reference);
    }

    public void setBandwidthUsage (float bandwidthUsage) {
      DashPlayer.dash_player_set_bandwidth_usage(this.reference, bandwidthUsage);
    }

    public void setMaxBitrate (int maxBitrate) {
      DashPlayer.dash_player_set_max_bitrate(this.reference, maxBitrate);
    }

    public void stop () {
      DashPlayer.dash_player_stop(this.reference);
    }

    /* c api */
    public static native long dash_player_jni_initialize();

    public static native void dash_player_play(long player);

    public static native void dash_player_set_uri(long player, String uri);

    public static native void dash_player_set_bandwidth_usage(long player, float bandwidth_usage);

    public static native void dash_player_set_max_bitrate(long player, int bitrate);
    
    public static native void dash_player_stop(long player);

    public static native void dash_player_unref(long player);

    /* sample usage */
    public static void main(String[] argv) {
      DashPlayer player = new DashPlayer();
      player.setUri("http://www-itec.uni-klu.ac.at/ftp/datasets/mmsys13/redbull_4sec.mpd");
      player.setBandwidthUsage((float) 1.0);
      player.setMaxBitrate(1024*1024);
      player.play();
      try { Thread.sleep(15000); } catch (InterruptedException e) {}
      player.stop();
    }

    public void finalize () {
      DashPlayer.dash_player_unref(this.reference);
    }
}
