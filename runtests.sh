#!/bin/bash

echo "Running..."
for file in ./tests/test_LPs/input/*;
do
    filename=${file##*/}

    echo ${filename}
    ./program < ./tests/test_LPs/input/${filename} > ./tests/output/test_LPs/${filename}
    if ! [[ $(head -1 ./tests/output/test_LPs/${filename}) = $(head -1 ./tests/test_LPs/output/${filename}) ]] || ! [[ $(head -2 ./tests/output/test_LPs/${filename}) = $(head -2 ./tests/test_LPs/output/${filename}) ]]; then
        diff -w ./tests/output/test_LPs/${filename} ./tests/test_LPs/output/${filename}
    fi
done

for file in ./tests/test_LPs_volume2/input/*;
do
    filename=${file##*/}

    echo ${filename}
    ./program < ./tests/test_LPs_volume2/input/${filename} > ./tests/output/test_LPs_volume2/${filename}
    if ! [[ $(head -1 ./tests/output/test_LPs_volume2/${filename}) = $(head -1 ./tests/output/test_LPs_volume2/${filename}) ]] || ! [[ $(head -2 ./tests/output/test_LPs_volume2/${filename}) = $(head -2 ./tests/output/test_LPs_volume2/${filename}) ]]; then
        diff -w ./tests/output/test_LPs_volume2/${filename} ./tests/output/test_LPs_volume2/${filename}
    fi
done

echo "DONE!"