set -x
autoreconf
./configure
make clean
make
javac DashPlayer.java
export CLASSPATH=`pwd`
javah DashPlayer



