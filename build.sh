# !/bin/sh

# set -xe

FLAGS="-g -std=c++11 -O3 -Wall -Wno-deprecated-declarations -Wno-unused-but-set-variable"

FRAMEWORKS="-framework CoreVideo -framework IOKit -framework Cocoa 
-framework GLUT -framework OpenGL"

LIBS="../raylibtest/lib/libraylib.a"

INCLUDE_PATHS="-I../raylibtest/lib"

clang++ $FLAGS $FRAMEWORKS rlib_rayray.cpp -o RayRay $INCLUDE_PATHS $LIBS