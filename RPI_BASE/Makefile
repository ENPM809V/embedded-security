PROJECT=$(realpath $(CURDIR))
SOURCEDIR=$(PROJECT)/srcs
BUILDDIR=$(PROJECT)/build
DIRS = Common Challenge Main
COMMON = $(SOURCEDIR)/Common
CHALLENGE = $(SOURCEDIR)/Challenge
MAIN = $(SOURCEDIR)/Main
SRCDIRS = $(foreach dir, $(DIRS), $(addprefix $(SOURCEDIR)/, $(dir)))
INCLUDES = $(foreach dir, $(SRCDIRS), $(addprefix -I, $(dir)))
COMMON_SRCS = $(wildcard $(COMMON)/*.c)
CHALLENGE_SRCS= $(wildcard $(CHALLENGE)/*.c)
MAIN_SRCS=$(wildcard $(MAIN)/*.c)
COMMON_OBJS = $(COMMON_SRCS:$(COMMON)/%.c=$(BUILDDIR)/%.o)
CHALLENGE_OBJS = $(CHALLENGE_SRCS:$(CHALLENGE)/%.c=$(BUILDDIR)/%.o)
MAIN_OBJS = $(MAIN_SRCS:$(MAIN)/%.c=$(BUILDDIR)/%.o)

SRCS = $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
OBJS = $(foreach dir,$(SRCDIRS), $(SRCS:$(dir)/%.c=$(BUILDDIR)/%.o))

$(info objects = $(COMMON_OBJS))
_DEPS = $(SOURCEDIR)/utils.h
DEPS = $(patsbust %,$(COMMON)/%,$(_DEPS))

VPATH = $(SRCDIRS)

KERN = kernel
TAR = hw

ifeq ($(RPI0), 1)
include Makefile.rpi0
endif

ifeq ($(RPI3), 1)
include Makefile.rpi3
endif

qemu-build: qemu $(KERN).img

clean:
	rm build/*.img build/$(KERN).elf build/$(KERN).dasm build/*.o >/dev/null 2>/dev/null || true