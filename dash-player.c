#include <gst/gst.h>
#include <glib.h>
#include "gob.h"

struct _DashPlayerPrivate
{
  char *uri;
  gulong bandwidth_usage;
  guint max_bitrate;

  GstPipeline *pipeline;
  GstElement *playbin;
  GstElement *videosink;
  GstElement *dashdemux;

};

G_DEFINE_TYPE_WITH_PRIVATE (DashPlayer, dash_player, G_TYPE_OBJECT)


/* Properties */

enum {
  PROP_0,
  PROP_URI,
  PROP_BANDWIDTH_USAGE,
  PROP_MAX_BITRATE,
  N_PROPERTIES
};


static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };


static void
dash_player_set_property (GObject *object, guint property_id,
    const GValue *value, GParamSpec *pspec)
{
  DashPlayer *self = DASH_PLAYER (object);

  switch (property_id) {
    case PROP_URI:
      self->priv->uri = g_value_dup_string (value);
      break;
    case PROP_BANDWIDTH_USAGE:
      self->priv->bandwidth_usage = g_value_get_ulong (value);
      break;
    case PROP_MAX_BITRATE:
      self->priv->max_bitrate = g_value_get_uint (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}


static void
dash_player_get_property (GObject *object, guint property_id,
    GValue *value, GParamSpec *pspec)
{
  DashPlayer *self = DASH_PLAYER (object);

  switch (property_id) {
    case PROP_URI:
      g_value_set_string (value, self->priv->uri);
      break;
    case PROP_BANDWIDTH_USAGE:
      g_value_set_ulong (value, self->priv->bandwidth_usage);
      break;
    case PROP_MAX_BITRATE:
      g_value_set_uint (value, self->priv->max_bitrate);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}


static void
dash_player_class_properties_init (DashPlayerClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->set_property = dash_player_set_property;
  gobject_class->get_property = dash_player_get_property;

  obj_properties[PROP_URI] = g_param_spec_string ("uri",
      "Uri of the dash media", "Uri of the dash media", "",
      G_PARAM_READWRITE);

  obj_properties[PROP_BANDWIDTH_USAGE] = g_param_spec_ulong ("bandwidth-usage",
      "Bandwidth Usage",
      "Percentage of the available bandwidth to use when selecting representations",
      0.0, G_MAXULONG, 0.0,
      G_PARAM_READWRITE);
  
  obj_properties[PROP_MAX_BITRATE] = g_param_spec_uint ("max-bitrate",
      "Max Bitrate",
      "Max of bitrate to be used when selecting representations",
      0.0, G_MAXUINT, 0.0,
      G_PARAM_READWRITE);

  g_object_class_install_properties (gobject_class, N_PROPERTIES,
      obj_properties);
}


/* object lifecycle */ 

static void
dash_player_class_init (DashPlayerClass *klass)
{
  dash_player_class_properties_init (klass);
}

void
dash_player_pipeline_build (GObject *object);

static void
dash_player_init (DashPlayer *self)
{
  /* dash_player_get_instance_private() is generated by G_DEFINE_TYPE_WITH_PRIVATE()
   * above, and it's local to the current compilation unit.
   */
  DashPlayerPrivate *priv = dash_player_get_instance_private (self);
  self->priv = priv;
  /* initialize public and private stuff to reasonable defaults */
  self->priv->pipeline = NULL;
  dash_player_pipeline_build (self);
}


static void
dash_player_dispose (GObject *gobject)
{
  DashPlayer *self = DASH_PLAYER (gobject);

  /* In dispose(), you are supposed to free all types referenced from this
   * object which might themselves hold a reference to self. Generally,
   * the most simple solution is to unref all members on which you own a 
   * reference.
   */

  /* dispose() might be called multiple times, so we must guard against
   * calling g_object_unref() on an invalid GObject by setting the member
   * NULL; g_clear_object() does this for us, atomically.
   */

  //g_clear_object (&self->priv->an_object);

  if (self->priv->pipeline != NULL) {
    gst_object_unref (GST_OBJECT (self->priv->pipeline));
    self->priv->pipeline = NULL;
  }

  /* Always chain up to the parent class; there is no need to check if
   * the parent class implements the dispose() virtual function: it is
   * always guaranteed to do so
   */

  G_OBJECT_CLASS (dash_player_parent_class)->dispose (gobject);
}


static void
dash_player_finalize (GObject *gobject)
{
  DashPlayer *self = DASH_PLAYER (gobject);

  //g_free (self->priv->a_string);
  /* Always chain up to the parent class; as with dispose(), finalize()
   * is guaranteed to exist on the parent's class virtual function table
   */
  G_OBJECT_CLASS (dash_player_parent_class)->finalize (gobject);
}


static void
on_element_added (GstBin *bin, GstElement *element, gpointer emitter) {
  gulong handle = 0;

  GstElementFactory *factory = gst_element_get_factory(element);
  GType type = gst_element_factory_get_element_type(factory);
  const char *name = g_type_name(type);

  g_print("%s added %s (%s) \n", gst_element_get_name(emitter),
      gst_element_get_name(element), name);

  if (g_strcmp0(name, "GstURIDecodeBin") == 0) {
    g_signal_connect(element, "element-added",
        G_CALLBACK(on_element_added), element);
    handle = g_signal_handler_find (emitter, G_SIGNAL_MATCH_FUNC,
        0, 0, NULL, G_CALLBACK(on_element_added), NULL);
    g_signal_handler_disconnect (emitter, handle); 
    return;
  }

  if (g_strcmp0(name, "GstDecodeBin") == 0) {
    g_signal_connect(element, "element-added",
        G_CALLBACK(on_element_added), element);
    handle = g_signal_handler_find (emitter, G_SIGNAL_MATCH_FUNC,
        0, 0, NULL, G_CALLBACK(on_element_added), NULL);
    g_signal_handler_disconnect (emitter, handle); 
    return;
  }

  if (g_strcmp0(name, "GstDashDemux") == 0) {
    g_object_set (element, "bandwidth-usage", 0.1, NULL);

    handle = g_signal_handler_find (emitter, G_SIGNAL_MATCH_FUNC,
        0, 0, NULL, G_CALLBACK(on_element_added), NULL);
    g_signal_handler_disconnect (emitter, handle); 
    return;
  }
}


void
dash_player_pipeline_build (GObject *object)
{
  GstPipeline *pipeline;
  GstElement *playbin;
  GstElement *videosink;
  GstElement *audiosink;
  GstBus *bus;

  DashPlayer *self = DASH_PLAYER (object);

  /* set up */
  videosink = gst_element_factory_make ("osxvideosink", "videosink");
  audiosink = gst_element_factory_make ("fakesink", "audiosink");
  playbin = gst_element_factory_make ("playbin", "pipeline");
  g_object_set (G_OBJECT (playbin), "uri", self->priv->uri, NULL);
  g_object_set (G_OBJECT (playbin), "video-sink", videosink, NULL);
  //g_object_set (G_OBJECT (playbin), "audio-sink", audiosink, NULL);

  pipeline = GST_PIPELINE(playbin);
  bus = gst_pipeline_get_bus (pipeline);
  //gst_bus_add_watch (bus, my_bus_callback, loop);
  g_signal_connect(playbin, "element-added",
      G_CALLBACK(on_element_added ), playbin);

  gst_object_unref (bus);

  self->priv->playbin = playbin;
  self->priv->videosink = videosink;
  self->priv->pipeline = pipeline;
  self->priv->dashdemux = NULL;
  
  //gst_object_unref (GST_OBJECT (self->priv->pipeline));
}

void
dash_player_play (GObject *object)
{
  DashPlayer *self = DASH_PLAYER (object);
  gst_element_set_state (self->priv->playbin, GST_STATE_PLAYING);
}

void
dash_player_stop (GObject *object)
{
  DashPlayer *self = DASH_PLAYER (object);
  gst_element_set_state (self->priv->playbin, GST_STATE_NULL);
}