#!bin/sh

rm -rf bin
mkdir bin
cd bin

proj_name=App
proj_root_dir=$(pwd)/../

flags=(
    -std=gnu99 -w -D__USE_MINGW_ANSI_STDIO=1
)

# Includes
inc=(
    -I ../third_party/include/
)

# Source files
src=(
    ../src/main.c
)

libs=(
    -lkernel32
    -luser32
    -lshell32
    -lgdi32
    -lAdvapi32
)

# Build
gcc -O0 ${inc[*]} ${src[*]} ${flags[*]} ${libs[*]} -lm -o ${proj_name}

cd ..
