#!/bin/bash

echo "Running..."
for file in ./test_LPs/input/vanderbei*;
do
    filename=${file##*/}

    echo ${filename}
    ./program < ./test_LPs/input/${filename} > ./output/test_LPs/${filename}
    diff -w ./output/test_LPs/${filename} ./test_LPs/output/${filename}
done

for file in ./test_LPs_volume2/input/*;
do
    filename=${file##*/}

    echo ${filename}
    ./program < ./test_LPs_volume2/input/${filename} > ./output/test_LPs_volume2/${filename}
    diff -w ./output/test_LPs_volume2/${filename} ./test_LPs_volume2/output/${filename}
done

echo "DONE!"
