#!/bin/bash -e

LIBS="core:runtime:1.1.1 lifecycle:runtime:1.1.1 lifecycle:livedata:1.1.1 lifecycle:livedata-core:1.1.1 lifecycle:viewmodel:1.1.1"
TARGET="android-28"
ANDROID="/home/tomber/android/android-sdk-linux_x86/tools/android"
MVNURL="https://maven.google.com/android/arch"

mkdir -p arch
cd arch

for l in ${LIBS}
do
    arr=(${l//:/ })
    lib=${arr[0]}
    name=${arr[1]}
    version=${arr[2]}
    mkdir -p ${lib}/${version}
    
    rm -Rf ${lib}/${version}/${name}
    mkdir -p ${lib}/${version}/${name}
    cd ${lib}/${version}/${name}
    echo "Downloading ${name}-${version}.aar"
    wget ${MVNURL}/${lib}/${name}/${version}/${name}-${version}.aar
    echo "Unpacking ${name}-${version}.aar"
    unzip -x ${name}-${version}.aar > /dev/null
    rm -f src
    mkdir -p libs src
    mv classes.jar libs/

    ${ANDROID} update lib-project -p `pwd` -t ${TARGET}
    echo "android.library=true" >> project.properties

    if [ "$name" = "livedata" ]
    then
	echo "android.library.reference.1=../../../lifecycle/${version}/livedata-core" >> project.properties
	echo "android.library.reference.2=../../../core/${version}/runtime" >> project.properties
    elif [ "$name" = "livedata-core" ]
    then
	echo "android.library.reference.1=../../../core/${version}/runtime" >> project.properties
    fi
    
    cd ../../..
done
