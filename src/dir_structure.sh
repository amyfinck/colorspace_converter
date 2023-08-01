#!/bin/sh
process_id=$!

mkdir -p ./input
mkdir -p ./output
mkdir -p ./output/luma
mkdir -p ./output/cb
mkdir -p ./output/cr
mkdir -p ./output/rgb

wait $process_id
echo Directory structure created