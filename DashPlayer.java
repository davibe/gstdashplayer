import java.awt.*;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class DashPlayer {

    static {
      // TODO: this hack is done because .loadLibrary seems not to work
      // correctly on macosx (mavericks at least), investigate it further
      String path = System.getProperty("java.library.path");
      try {
        System.load(path + "/.libs/libdashplayer.dylib");
      } catch (java.lang.UnsatisfiedLinkError e) {
        System.load(path + "/.libs/libdashplayer.so");
      }
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

    public void setWindowHandle (long handle) {
      DashPlayer.dash_player_set_window_handle(this.reference, handle);
    }

    /* c api */

    public static native long gst_init();

    public static native long dash_player_jni_initialize();

    public static native void dash_player_play(long player);

    public static native void dash_player_set_uri(long player, String uri);

    public static native void dash_player_set_bandwidth_usage(long player, float bandwidth_usage);

    public static native void dash_player_set_max_bitrate(long player, int bitrate);
    
    public static native void dash_player_stop(long player);

    public static native void dash_player_unref(long player);

    public static native void dash_player_set_window_handle(long player, long handle);


    // instead of using JNA (com.java.jna.Native.getComponentID)
    // we use these two hacky functions

    protected static Object invokeMethod(Object o, String methodName)
        throws IllegalAccessException, IllegalArgumentException,
        InvocationTargetException {
      Class c = o.getClass();

      for (Method m : c.getMethods()) {
        if (m.getName().equals(methodName)) {
          Object ret = m.invoke(o);
          return ret;
        }
      }

      throw new RuntimeException("Could not find method named '" + 
          methodName+"' on class " + c);
    }

    protected static long getComponentID(Component component) {
      java.awt.peer.ComponentPeer peer;
      long handle;

      try {
        peer = component.getPeer();
        handle = invokeMethod(peer, "getWindow");
        return handle;
      } catch (IllegalAccessException e) {
      } catch (IllegalArgumentException e) {
      } catch (InvocationTargetException e) {
      }

      return 0;
    }


    /* sample usage */

    public static void main(String[] argv) {
      // we just do this once for the whole appication
      DashPlayer.gst_init();

      // then create the object
      DashPlayer player = new DashPlayer();
      // set parameters
      player.setUri("http://www-itec.uni-klu.ac.at/ftp/datasets/mmsys13/redbull_4sec.mpd");
      player.setBandwidthUsage((float) 1.0);
      player.setMaxBitrate(1024*1024);
      
      // create a GUI with a canvas where we will render the video
      Frame mainFrame = new Frame("Java AWT Examples");
      mainFrame.setSize(400, 400);
      mainFrame.setLayout(new GridLayout(1, 1));
      Canvas canvas = new Canvas();
      canvas.setBackground(Color.RED);
      mainFrame.add(canvas);
      mainFrame.setVisible(true);

      long handle = getComponentID(canvas);
      player.setWindowHandle(handle);

      player.play();
      try { Thread.sleep(15000); } catch (InterruptedException e) {}
      player.stop();
      try { Thread.sleep(5000); } catch (InterruptedException e) {}
      player.play();
      try { Thread.sleep(15000); } catch (InterruptedException e) {}
      player.stop();
    }

    public void finalize () {
      DashPlayer.dash_player_unref(this.reference);
    }
}
