# Cyclone 2000

This is the open source version of the Android game Cyclone 2000,
which is a clone of the Tempest 2000 game, originally made by Jeff
Minter on the Atari Jaguar. It is the same game that is published as
on the Google Play Store as
https://play.google.com/store/apps/details?id=org.nocrew.tomas.cyclone2000.full
or
https://play.google.com/store/apps/details?id=org.nocrew.tomas.cyclone2000.demo,
but does not include any ads, in-app purchase or Google Play services
functionality.

## Building

To build, the _local.properties_ file need to include *sdk.dir* and
*ndk.dir* entries. Java 8 seems to be required, it doesn't compile
properly with Java 11. The build system is based on GNU make and uses
_ant_ to build. There is no direct support to build from Android
Studio or using _gradle_ at this point.

Because of this, support libraries need to be downloaded and setup
first. Before compiling the first time, run:
```make setup```

To build an APK, run:
```make release```

To get a signed APK, you also need to include the *key.store* and
*key.alias* arguments in the _local.properties_ file.

## Porting

Most of the game engine is written in C as a JNI library. The Android
specific parts are written in Java and includes code for touch,
controller, settings, sound, etc.
