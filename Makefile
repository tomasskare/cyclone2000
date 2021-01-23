ADB = /home/tomber/android/android-sdk-linux_x86/platform-tools/adb

PKG=org.nocrew.tomas.cyclone2000

EXTLIBS = libs/commons-lang3-3.1.jar		\
	  libs/cardboard.jar			\
	  libs/libprotobuf-java-2.3-nano.jar	\
	  libs/support-annotations-28.0.0.jar	\
	  libs/collections-28.0.0.jar		\
	  libs/core-common-1.1.1.jar		\
	  libs/lifecycle-common-1.1.1.jar

libs/%.jar: extlibs/%.jar
	mkdir -p libs
	cp extlibs/$(notdir $<) libs/

common-libs: $(EXTLIBS)

jni:
	ant jni

clean:
	ant jni-clean

realclean: clean
	rm -Rf gen bin libs obj tmplibs

setup:
	(cd util ; ./setup-arch-download.sh ; ./setup-support-download.sh)

compile: common-libs
	ant release

install: common-libs
	ant release
	$(ADB) install -r -d bin/Cyclone2000-release.apk

release: realclean common-libs
	mkdir -p libs
	cp extlibs/*.jar libs/
	ant release
	mkdir -p release
	cp bin/Cyclone2000-release.apk release/

run: install
	$(ADB) shell am start -a android.intent.action.MAIN -n $(PKG)/.Cyclone2000Main

run-gp: install
	$(ADB) shell am start -a android.intent.action.MAIN -n $(PKG)/.GamepadGuideActivity

run-set: install
	$(ADB) shell am start -a android.intent.action.MAIN -n $(PKG)/.C2Preferences
