/*
 * Copyright/Licensing information.
 */

/* inclusion guard */
#ifndef __DASH_PLAYER_H__
#define __DASH_PLAYER_H__

#include <glib-object.h>
/*
 * Potentially, include other headers on which this header depends.
 */

/*
 * Type macros.
 */
#define DASH_TYPE_PLAYER                  (dash_player_get_type ())
#define DASH_PLAYER(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DASH_TYPE_PLAYER, DashPlayer))
#define DASH_IS_PLAYER(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DASH_TYPE_PLAYER))
#define DASH_PLAYER_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), DASH_TYPE_PLAYER, DashPlayerClass))
#define DASH_IS_PLAYER_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), DASH_TYPE_PLAYER))
#define DASH_PLAYER_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), DASH_TYPE_PLAYER, DashPlayerClass))

typedef struct _DashPlayer        DashPlayer;
typedef struct _DashPlayerClass   DashPlayerClass;
typedef struct _DashPlayerPrivate DashPlayerPrivate;

struct _DashPlayer
{
  /* Parent instance structure */
  GObject parent_instance;

  /* instance members */

  DashPlayerPrivate *priv;
};

struct _DashPlayerClass
{
  /* Parent class structure */
  GObjectClass parent_class;

  /* class members */
};

/* used by DASH_TYPE_PLAYER */
GType dash_player_get_type (void);

/*
 * Method definitions.
 */

void
dash_player_play (DashPlayer *self);

void
dash_player_stop (DashPlayer *self);


#endif /* __DASH_PLAYER_H__ */
