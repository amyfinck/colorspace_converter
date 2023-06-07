#! /bin/bash

mkdir -p /tmp/tmp_dir/
cp ./main.c /tmp/tmp_dir/

process_id=$!

/opt/arm/4.3.2/bin/arm-linux-gcc -mfloat-abi=softfp -mfpu=neon -static -O3 -S /tmp/tmp_dir/main.c
/opt/arm/4.3.2/bin/arm-linux-gcc -mfloat-abi=softfp -mfpu=neon -static -O3 -S /tmp/tmp_dir/main.c -o main.exe

wait $process_id