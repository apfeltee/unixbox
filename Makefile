
libname = unixbox
exename = unixbox
libfile = lib$(libname).a

# sources for tinyjs
sources_exe = src/main.c
headerfiles  = $(wildcard src/*.h)
sources_lib = \
	$(wildcard src/programs/*.c) \
	src/common.c \
	src/gopt.c

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
WFLAGS = \
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

CFLAGS = $(INCFLAGS) $(WFLAGS) -c -g3 -ggdb -rdynamic -D_DEBUG
LDFLAGS = -g -rdynamic -L. -Wl,-rpath=.
BINFLAGS = $(libfile) $(LDFLAGS)

##########################
#### tasks begin here ####
##########################

RM = rm -fv

all: $(objects_lib) $(libfile) $(exename) $(headerfiles)

$(libfile): $(sources_lib) $(objects_lib) $(headerfiles)
	ar rcs $(libfile) $(objects_lib)

$(exename): $(libfile) $(sources) $(sources_exe) $(objects_exe)
	$(CXX) -o $(exename) $(objects_exe) $(BINFLAGS)

clean:
	@$(RM) $(objects_lib)
	@$(RM) $(objects_exe)

distclean: clean
	@$(RM) $(libfile)
	@$(RM) $(exename)

rebuild: distclean all

.c.o:
	$(CC) $(CFLAGS) $< -o $@
