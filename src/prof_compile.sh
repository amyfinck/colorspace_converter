#! /bin/bash
process_id=$!

echo Copying files to /tmp/tmp_dir/

mkdir -p /tmp/tmp_dir/
cp ./colorspace_converter.c /tmp/tmp_dir/
cp ./colorspace_converter.h /tmp/tmp_dir/

cp ./bmp_parser.h /tmp/tmp_dir/
cp ./bmp_parser.c /tmp/tmp_dir/

cp ./constants.h /tmp/tmp_dir/

cp ./converters.h /tmp/tmp_dir/
cp ./converters.c /tmp/tmp_dir/

cp ./transformers.h /tmp/tmp_dir/
cp ./transformers.c /tmp/tmp_dir/

cp ./utilities.h /tmp/tmp_dir/

echo Compiling unoptimized colorspace converter
/opt/arm/4.3.2/bin/arm-linux-gcc -pg -static -mfpu=neon -O3 /tmp/tmp_dir/colorspace_converter.c /tmp/tmp_dir/bmp_parser.c /tmp/tmp_dir/converters.c /tmp/tmp_dir/transformers.c cc.exe

wait $process_id

echo Compile complete, output is cc.exe
