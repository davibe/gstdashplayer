#include "DashPlayer.h"
#include "dash-player.c"


JNIEXPORT jlong JNICALL Java_DashPlayer_gst_1init
  (JNIEnv *env, jclass class)
{
  gst_init (NULL, NULL);
}


JNIEXPORT jlong JNICALL Java_DashPlayer_dash_1player_1jni_1initialize
  (JNIEnv *env, jclass class)
{
  DashPlayer *player = g_object_new(DASH_TYPE_PLAYER, NULL);
  return (jlong) player;
}


JNIEXPORT void JNICALL Java_DashPlayer_dash_1player_1play
  (JNIEnv *env, jclass class, jlong reference)
{
  DashPlayer *player = (DashPlayer *) reference;
  dash_player_play (player);
}


JNIEXPORT void JNICALL Java_DashPlayer_dash_1player_1set_1uri
  (JNIEnv *env, jclass class, jlong reference, jstring juri)
{
  DashPlayer *player = (DashPlayer *) reference;
  const char *uri = (*env)->GetStringUTFChars(env, juri, 0);  
  g_object_set (G_OBJECT (player), "uri", uri, NULL);
}


JNIEXPORT void JNICALL Java_DashPlayer_dash_1player_1set_1bandwidth_1usage
  (JNIEnv *env, jclass class, jlong reference, jfloat jbandwidth_usage)
{
  DashPlayer *player = (DashPlayer *) reference;
  g_object_set (G_OBJECT (player), "bandwidth-usage",
      (gulong) jbandwidth_usage, NULL);
}


JNIEXPORT void JNICALL Java_DashPlayer_dash_1player_1set_1max_1bitrate
  (JNIEnv *env, jclass class, jlong reference, jint jmax_bitrate)
{
  DashPlayer *player = (DashPlayer *) reference;
  g_object_set (G_OBJECT (player), "max-bitrate",
      (guint) jmax_bitrate, NULL);
}


JNIEXPORT void JNICALL Java_DashPlayer_dash_1player_1stop
  (JNIEnv *env, jclass class, jlong reference)
{
  DashPlayer *player = (DashPlayer *) reference;
  dash_player_stop (player);
}


JNIEXPORT void JNICALL Java_DashPlayer_dash_1player_1unref
  (JNIEnv *env, jclass class, jlong reference)
{
  DashPlayer *player = (DashPlayer *) reference;
  g_object_unref (player);
}


JNIEXPORT void JNICALL Java_DashPlayer_dash_1player_1set_1window_1handle
  (JNIEnv *env, jclass class, jlong reference, jlong handle)
{
  DashPlayer *player = (DashPlayer *) reference;
  g_object_set (G_OBJECT (player), "window-handle",
      (glong) handle, NULL);
}
