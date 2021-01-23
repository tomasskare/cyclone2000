#!/bin/bash -e

LIBS="design:28.0.0 transition:28.0.0 animated-vector-drawable:28.0.0 support-vector-drawable:28.0.0 support-v4:28.0.0 appcompat-v7:28.0.0 preference-v7:28.0.0 recyclerview-v7:28.0.0 preference-v14:28.0.0 support-compat:28.0.0 support-media-compat:28.0.0 support-core-utils:28.0.0 support-core-ui:28.0.0 support-fragment:28.0.0 versionedparcelable:28.0.0 documentfile:28.0.0 loader:28.0.0 localbroadcastmanager:28.0.0 print:28.0.0 customview:28.0.0 viewpager:28.0.0 coordinatorlayout:28.0.0 drawerlayout:28.0.0 slidingpanelayout:28.0.0 interpolator:28.0.0 swiperefreshlayout:28.0.0 asynclayoutinflater:28.0.0 cursoradapter:28.0.0 cardview-v7:28.0.0"
TARGET="android-28"
ANDROID="/home/tomber/android/android-sdk-linux_x86/tools/android"
MVNURL="https://maven.google.com/com/android/support"
ARCHVERSION="1.1.1"

mkdir -p support
cd support

for l in ${LIBS}
do
    arr=(${l//:/ })
    name=${arr[0]}
    version=${arr[1]}
    mkdir -p ${version}
    
    rm -Rf ${version}/${name}
    mkdir -p ${version}/${name}
    cd ${version}/${name}
    echo "Downloading ${name}-${version}.aar"
    wget ${MVNURL}/${name}/${version}/${name}-${version}.aar
    echo "Unpacking ${name}-${version}.aar"
    unzip -x ${name}-${version}.aar > /dev/null
    rm -f src
    mkdir -p libs src
    mv classes.jar libs/

    ${ANDROID} update lib-project -p `pwd` -t ${TARGET}
    echo "android.library=true" >> project.properties

    if [ "$name" = "appcompat-v7" ]
    then
	echo "android.library.reference.1=../../${version}/support-compat" >> project.properties
	echo "android.library.reference.2=../../${version}/animated-vector-drawable" >> project.properties
	echo "android.library.reference.3=../../${version}/support-vector-drawable" >> project.properties
	echo "android.library.reference.4=../../${version}/cursoradapter" >> project.properties
	echo "android.library.reference.5=../../${version}/support-core-utils" >> project.properties
	echo "android.library.reference.6=../../${version}/support-fragment" >> project.properties
    elif [ "$name" = "support-v4" ]
    then
	echo "android.library.reference.1=../../${version}/support-compat" >> project.properties
	echo "android.library.reference.2=../../${version}/support-media-compat" >> project.properties
	echo "android.library.reference.3=../../${version}/support-core-utils" >> project.properties
	echo "android.library.reference.4=../../${version}/support-core-ui" >> project.properties
	echo "android.library.reference.5=../../${version}/support-fragment" >> project.properties
    elif [ "$name" = "design" ]
    then
	echo "android.library.reference.1=../../${version}/support-v4" >> project.properties
	echo "android.library.reference.2=../../${version}/appcompat-v7" >> project.properties
	echo "android.library.reference.3=../../${version}/recyclerview-v7" >> project.properties
	echo "android.library.reference.4=../../${version}/transition" >> project.properties
	echo "android.library.reference.5=../../${version}/cardview-v7" >> project.properties
    elif [ "$name" = "support-core-utils" ]
    then
	echo "android.library.reference.1=../../${version}/support-compat" >> project.properties
	echo "android.library.reference.2=../../${version}/documentfile" >> project.properties
	echo "android.library.reference.3=../../${version}/loader" >> project.properties
	echo "android.library.reference.4=../../${version}/localbroadcastmanager" >> project.properties
	echo "android.library.reference.5=../../${version}/print" >> project.properties
    elif [ "$name" = "support-core-ui" ]
    then
	echo "android.library.reference.1=../../${version}/support-compat" >> project.properties
	echo "android.library.reference.2=../../${version}/support-core-utils" >> project.properties
	echo "android.library.reference.3=../../${version}/customview" >> project.properties
	echo "android.library.reference.4=../../${version}/viewpager" >> project.properties
	echo "android.library.reference.5=../../${version}/coordinatorlayout" >> project.properties
	echo "android.library.reference.6=../../${version}/drawerlayout" >> project.properties
	echo "android.library.reference.7=../../${version}/slidingpanelayout" >> project.properties
	echo "android.library.reference.8=../../${version}/interpolator" >> project.properties
	echo "android.library.reference.9=../../${version}/swiperefreshlayout" >> project.properties
	echo "android.library.reference.10=../../${version}/asynclayoutinflater" >> project.properties
	echo "android.library.reference.11=../../${version}/cursoradapter" >> project.properties
    elif [ "$name" = "support-media-compat" ]
    then
	echo "android.library.reference.1=../../${version}/support-compat" >> project.properties
    elif [ "$name" = "support-compat" ]
    then
	echo "android.library.reference.1=../../../arch/lifecycle/${ARCHVERSION}/runtime" >> project.properties
	echo "android.library.reference.2=../../${version}/versionedparcelable" >> project.properties
    elif [ "$name" = "loader" ]
    then
	echo "android.library.reference.1=../../../arch/lifecycle/${ARCHVERSION}/livedata" >> project.properties
	echo "android.library.reference.1=../../../arch/lifecycle/${ARCHVERSION}/livedata-core" >> project.properties
	echo "android.library.reference.2=../../../arch/lifecycle/${ARCHVERSION}/viewmodel" >> project.properties
    elif [ "$name" = "preference-v7" ]
    then
	echo "android.library.reference.1=../../${version}/support-compat" >> project.properties
	echo "android.library.reference.2=../../${version}/support-fragment" >> project.properties
	echo "android.library.reference.3=../../${version}/appcompat-v7" >> project.properties
	echo "android.library.reference.4=../../${version}/recyclerview-v7" >> project.properties
    elif [ "$name" = "preference-v14" ]
    then
	echo "android.library.reference.1=../../${version}/preference-v7" >> project.properties
    elif [ "$name" = "support-fragment" ]
    then
	echo "android.library.reference.1=../../${version}/support-compat" >> project.properties
	echo "android.library.reference.2=../../${version}/support-core-ui" >> project.properties
	echo "android.library.reference.3=../../${version}/support-core-utils" >> project.properties
	echo "android.library.reference.5=../../../arch/lifecycle/${ARCHVERSION}/viewmodel" >> project.properties
	echo "android.library.reference.6=../../${version}/loader" >> project.properties
    fi

    cd ../..
done
