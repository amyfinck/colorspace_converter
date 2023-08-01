#! /bin/bash
process_id=$!

echo Copying files to /tmp/tmp_comp/

mkdir -p /tmp/tmp_comp/
cp ./colorspace_converter.c /tmp/tmp_comp/
cp ./colorspace_converter.h /tmp/tmp_comp/

cp ./bmp_parser.h /tmp/tmp_comp/
cp ./bmp_parser.c /tmp/tmp_comp/

cp ./constants.h /tmp/tmp_comp/

cp ./converters.h /tmp/tmp_comp/
cp ./converters.c /tmp/tmp_comp/

cp ./transformers.h /tmp/tmp_comp/
cp ./transformers.c /tmp/tmp_comp/

cp ./utilities.h /tmp/tmp_comp/
cp ./utilities.c /tmp/tmp_comp/

echo Compiling unoptimized colorspace converter

/opt/arm/4.3.2/bin/arm-linux-gcc -pg -static -mfpu=neon -O3 -I /tmp/tmp_comp /tmp/tmp_comp/colorspace_converter.c /tmp/tmp_comp/bmp_parser.c /tmp/tmp_comp/converters.c /tmp/tmp_comp/transformers.c /tmp/tmp_comp/utilities.c -o cc.exe


wait $process_id

echo Compile complete, output is cc.exe
