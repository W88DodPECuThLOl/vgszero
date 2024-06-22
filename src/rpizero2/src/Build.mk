CIRCLEHOME = ./circle
CHECK_DEPS = 1 
STANDARD = -std=c++17
CFLAGS = -O3
CFLAGS += -I../../core
CFLAGS += -I../../core/nsf
CFLAGS += -I../../core/nsf/xgm
CFLAGS += -I../../core/nsf/xgm/devices
CFLAGS += -I../../core/nsf/xgm/devices/Audio
CFLAGS += -I../../core/nsf/xgm/devices/CPU
CFLAGS += -I../../core/nsf/xgm/devices/CPU/km6502
CFLAGS += -I../../core/nsf/xgm/devices/Memory
CFLAGS += -I../../core/nsf/xgm/devices/Misc
CFLAGS += -I../../core/nsf/xgm/devices/Sound
CFLAGS += -I../../core/nsf/xgm/player/nsf
CPPFLAGS = $(CFLAGS)
CPPFLAGS += -DZ80_DISABLE_DEBUG
CPPFLAGS += -DZ80_DISABLE_BREAKPOINT
CPPFLAGS += -DZ80_DISABLE_NESTCHECK
CPPFLAGS += -DZ80_CALLBACK_WITHOUT_CHECK
CPPFLAGS += -DZ80_CALLBACK_PER_INSTRUCTION
CPPFLAGS += -DZ80_UNSUPPORT_16BIT_PORT
CPPFLAGS += -DZ80_NO_FUNCTIONAL
CPPFLAGS += -DZ80_NO_EXCEPTION
CPPFLAGS += -D_TIME_T_DECLARED
CPPFLAGS += -DARM_ALLOW_MULTI_CORE
OBJS = main.o
OBJS += std.o
OBJS += kernel.o
OBJS += multicoremanager.o
OBJS += splash.o
OBJS += sderror.o
OBJS += vgstone.o
OBJS += vgs0math.o
OBJS += nsf.o
OBJS += rconv.o
OBJS += nes_cpu.o
OBJS += nes_bank.o
OBJS += nes_mem.o
OBJS += nsf2_vectors.o
OBJS += nsf2_irq.o
OBJS += nes_apu.o
OBJS += nes_dmc.o
OBJS += nes_vrc6.o
OBJS += rom_tndtable.o
LIBS = $(CIRCLEHOME)/lib/libcircle.a
LIBS += $(CIRCLEHOME)/lib/fs/libfs.a
LIBS += $(CIRCLEHOME)/lib/input/libinput.a
LIBS += $(CIRCLEHOME)/lib/sched/libsched.a
LIBS += $(CIRCLEHOME)/lib/sound/libsound.a
LIBS += $(CIRCLEHOME)/lib/usb/libusb.a
LIBS += $(CIRCLEHOME)/addon/fatfs/libfatfs.a
LIBS += $(CIRCLEHOME)/addon/linux/liblinuxemu.a
LIBS += $(CIRCLEHOME)/addon/SDCard/libsdcard.a
LIBS += $(CIRCLEHOME)/addon/vc4/vchiq/libvchiq.a
LIBS += $(CIRCLEHOME)/addon/vc4/sound/libvchiqsound.a
include $(CIRCLEHOME)/Rules.mk
-include $(DEPS)
