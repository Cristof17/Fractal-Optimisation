#!/bin/bash

HWDIR=homework
RUNS=1

prepare() {
    #build the ckecker
    make

    #prepare the homework for testing
    mkdir $HWDIR
    mv *.zip $HWDIR
    cp checker $HWDIR
    cd $HWDIR
    unzip *.zip
    cp ../../fractal ./
    cd ..
}

build_serial() {
    cd $HWDIR
    make clean
    make
    cd ..
}

build_parallel() {
    cd $HWDIR
    make clean
    make parallel
    cd ..
}

run_homework() {
    cd $HWDIR
    for i in `seq 1 $RUNS`;
    do
        echo "###########"
        echo "Iteration $i"
        echo "###########"

        #measure the time
        START_BENCHMARK=$(date +"%s%N")
        ./fractal
        END_BENCHMARK=$(date +"%s%N")
            
        #check the results
        echo ""
        for index in `seq 101 105`;
        do
            echo "Checking Image-$index.ppm"
            echo "----------------------"
            ./checker ../Image-$index.ppm Image-$index.ppm
            echo ""
        done
        
        let "RUNNING_TIME=$END_BENCHMARK-$START_BENCHMARK"
        echo ">>>>>> Iteration $i done in $RUNNING_TIME nanoseconds <<<<<<"
        echo ""
    done
    cd ..
}

cleanup() {
    rm -rf $HWDIR
    make clean    
}
