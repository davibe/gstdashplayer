rm example
gcc dash-player.c example.c -o example -Xlinker \
  $(pkg-config --libs gstreamer-1.0 glib-2.0) \
  -Wall -ggdb -O0 \
  $(pkg-config --cflags --libs gstreamer-1.0 glib-2.0)

export DISPLAY=:0
xhost +
GST_DEBUG=*:0 ./example
rm example

