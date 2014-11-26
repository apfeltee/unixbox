
libname = unixbox
exename = unixbox
libfile = lib$(libname).a

script      =  generate.sh
headerfiles =  src/private.h
generated   =  src/funcs.h
sources_exe =  src/main.c
sources_lib =  $(wildcard src/programs/*.c)
sources_lib += src/common.c
sources_lib += src/gopt.c

# names for intermediate files
objects_lib     = $(sources_lib:.c=.o)
objects_exe     = $(sources_exe:.c=.o)

#############################################
### warning: edit below at your own risk! ###
#############################################

CC = clang

# flags for header file paths
INCFLAGS = -Isrc

# remove underscore for heavy-duty checks.
# WARNING: VERBOSE
_WFLAGS = \
	-pedantic -Wall -Wextra \
	-Wcast-align \
	-Wcast-qual \
	-Wctor-dtor-privacy \
	-Wdisabled-optimization \
	-Winit-self \
	-Wmissing-declarations \
	-Wold-style-cast \
	-Woverloaded-virtual \
	-Wredundant-decls \
	-Wshadow \
	-Wsign-conversion \
	-Wsign-promo \
	-Wstrict-overflow=5 \
	-Wswitch-default \
	-Wundef \
	-Wno-unused

WFLAGS = -Wall -Wextra -pedantic

CFLAGS = $(INCFLAGS) $(WFLAGS) -g3 -ggdb
LDFLAGS = -lm -ldl
BINFLAGS = $(libfile) $(LDFLAGS)

##########################
#### tasks begin here ####
##########################

RM = rm -fv

all: $(generated) $(objects_lib) $(libfile) $(exename) $(headerfiles)

$(generated): $(script) $(sources_lib)
	bash $(script) > $(generated)

$(libfile): $(sources_lib) $(objects_lib) $(headerfiles)
	ar rcs $(libfile) $(objects_lib)

$(exename): $(libfile) $(sources) $(sources_exe) $(objects_exe)
	$(CC) -o $(exename) $(objects_exe) $(BINFLAGS)

clean:
	@$(RM) $(objects_lib)
	@$(RM) $(objects_exe)
	@$(RM) $(generated)

distclean: clean
	@$(RM) $(libfile)
	@$(RM) $(exename)

rebuild: distclean all

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

