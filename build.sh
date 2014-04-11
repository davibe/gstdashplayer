set -x
autoreconf
./configure
make clean
make
export CLASSPATH=`pwd`:/usr/share/java/jna.jar
javac DashPlayer.java
export CLASSPATH=`pwd`
javah DashPlayer



