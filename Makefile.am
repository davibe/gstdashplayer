lib_LTLIBRARIES = libdashplayer.la
libdashplayer_la_CFLAGS = $(GST_CFLAGS) $(GLIB_CFLAGS) $(GSTVIDEO_CFLAGS) -ggdb -O0
libdashplayer_la_CFLAGS += -I/System/Library/Frameworks/JavaVM.framework/Headers/
libdashplayer_la_CFLAGS += -I/usr/lib/jvm/java-6-openjdk-amd64/include
#libdashplayer_la_CFLAGS += -I/usr/lib/jvm/java-6-oracle/include/linux
libdashplayer_la_SOURCES = DashPlayer.c #dash-player.c
libdashplayer_la_LDFLAGS = $(GST_LIBS) $(GLIB_LIBS) $(GSTVIDEO_LIBS)


#bin_PROGRAMS = example		
#example_SOURCES = example.c dash-player.c
#example_CPPFLAGS = $(GST_CFLAGS) $(GLIB_CFLAGS)
#example_CFLAGS = $(GST_CFLAGS) $(GLIB_CFLAGS) -I/System/Library/Frameworks/JavaVM.framework/Headers/
#example_LDFLAGS= $(GST_LIBS) $(GLIB_LIBS) -ggdb -O0
#example_LDADD= $(GST_LIBS) $(GLIB_LIBS)

