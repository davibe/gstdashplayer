#include <gst/gst.h>
#include <glib.h>
#include "dash-player.h"


gboolean
after_one_second (gpointer user_data)
{
  DashPlayer *player = (DashPlayer *) user_data;
  dash_player_play (player);

  return FALSE;
}

int
main (int   argc,
      char *argv[])
{

  /* initialize the gstreamer subsystem */ 
  gst_init (NULL, NULL);
  
  /* create the dash player */ 
  DashPlayer *player = g_object_new (DASH_TYPE_PLAYER, NULL);

  /* set some props */ 
  char *uri =
    "http://www-itec.uni-klu.ac.at/ftp/datasets/mmsys13/redbull_4sec.mpd";

  g_object_set (G_OBJECT (player), "uri", uri, NULL);
  g_object_set (G_OBJECT (player), "bandwidth-usage", (gulong) 1.0, NULL);
  g_object_set(G_OBJECT (player), "max-bitrate", (guint) 1024*1024, NULL);

  g_timeout_add (1*1000, after_one_second, player);

  /* run the main loop (blocking)*/ 
  GMainLoop *loop;
  loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (loop);
  return 0;
}

