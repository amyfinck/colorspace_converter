#! /bin/bash
process_id=$!

echo Copying files to /tmp/tmp_dir/

mkdir -p /tmp/tmp_dir/
cp ./main.c /tmp/tmp_dir/
# add more files as they are added to our project

echo Compiling main.c
/opt/arm/4.3.2/bin/arm-linux-gcc -static -mfloat-abi=softfp -mfpu=neon -O3 /tmp/tmp_dir/main.c -o main.exe

wait $process_id

echo Compile complete, output is main.exe