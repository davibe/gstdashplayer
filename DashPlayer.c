#include "DashPlayer.h"
#include "dash-player.c"

/*
 * Class:     DashPlayer
 * Method:    dash_player_jni_initialize
 * Signature: ()V
 */

DashPlayer *player = NULL;

JNIEXPORT void JNICALL Java_DashPlayer_dash_1player_1jni_1initialize
  (JNIEnv *env, jclass class)
{
  gst_init (NULL, NULL);
  player = g_object_new(DASH_TYPE_PLAYER, NULL);
  char *uri =
    "http://www-itec.uni-klu.ac.at/ftp/datasets/mmsys13/redbull_4sec.mpd";

  g_object_set (G_OBJECT (player), "uri", uri, NULL);
  g_object_set (G_OBJECT (player), "bandwidth-usage", (gulong) 1.0, NULL);
  g_object_set (G_OBJECT (player), "max-bitrate", (guint) 1024*1024, NULL);

}

/*
 * Class:     DashPlayer
 * Method:    dash_player_play
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_DashPlayer_dash_1player_1play
  (JNIEnv *env, jclass class)
{
  dash_player_play (player);
}


