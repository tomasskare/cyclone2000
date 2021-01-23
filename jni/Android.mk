LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cyclone2000

LOCAL_CFLAGS := -g -Werror -Wall

ifdef NDK_DEBUG
LOCAL_CFLAGS += -O0
else
LOCAL_CFLAGS += -O2
endif

LOCAL_SRC_FILES := native_main.c menu.c engine.c orthoquad.c gameover.c \
		   gamearea.c starfield.c score.c life.c control.c line.c \
		   player.c player_fire.c player_death.c superzapper.c \
		   powerup.c banner.c text.c ai_droid.c highscore.c \
		   matrix.c shaders.c \
		   foe_flipper.c foe_tanker_flipper.c \
		   foe_fire.c foe_explosion.c foe_explosion_sphere.c \
		   foe_spiker.c foe_spikes.c foe_demonhead.c \
		   foe_fuseball.c foe_pulsar.c foe_tanker_fuseball.c \
		   foe_tanker_pulsar.c foe_mirror.c foe_mirror_fire.c \
		   foe_ufo.c foe_ufo_fire.c

LOCAL_LDLIBS := -lGLESv2 -ldl -llog

include $(BUILD_SHARED_LIBRARY)
