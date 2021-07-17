#!/bin/bash

echo "Running..."
for file in ./test_LPs/input/*;
do
    filename=${file##*/}

    echo ${filename}
    ./program < ./test_LPs/input/${filename} > ./output/test_LPs/${filename}

    if ! [[ $(head -1 ./output/test_LPs/${filename}) = $(head -1 ./test_LPs/output/${filename}) ]] || ! [[ $(head -2 ./output/test_LPs/${filename}) = $(head -2 ./test_LPs/output/${filename}) ]]; then
        diff -w ./output/test_LPs/${filename} ./test_LPs/output/${filename}
    fi
done

for file in ./test_LPs_volume2/input/*;
do
    filename=${file##*/}

    echo ${filename}
    ./program < ./test_LPs_volume2/input/${filename} > ./output/test_LPs_volume2/${filename}
    if ! [[ $(head -1 ./output/test_LPs_volume2/${filename}) = $(head -1 ./output/test_LPs_volume2/${filename}) ]] || ! [[ $(head -2 ./output/test_LPs_volume2/${filename}) = $(head -2 ./output/test_LPs_volume2/${filename}) ]]; then
        diff -w ./output/test_LPs_volume2/${filename} ./output/test_LPs_volume2/${filename}
    fi
done

echo "DONE!"
