TARGET = tc4-tribute3

VERSION = 4.3.1

DEL_FILE = rm -f

CC      = g++
CFLAGS = -pipe -Wall -g #-"ggdb"
SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LIBS = $(shell sdl2-config --libs) 

SDL_TTF_LIBS	= -lSDL2_ttf
SDL_IMAGE_LIBS	= -lSDL2_image
SDL_MIXER_LIBS  = -lSDL2_mixer

OBJECTS = src/main.o \
          src/audio.o \
          src/data.o \
          src/input.o \
          src/interface.o \
          src/logic.o \
          src/screens.o \
          src/visuals.o

SOURCES = src/main.cpp \
          src/audio.cpp \
          src/data.cpp \
          src/input.cpp \
          src/interface.cpp \
          src/logic.cpp \
          src/screens.cpp \
          src/visuals.cpp

HEADERS = src/audio.h \
          src/data.h \
          src/input.h \
          src/interface.h \
          src/logic.h \
          src/screens.h \
          src/visuals.h

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(SDL_LIBS) $(SDL_TTF_LIBS) $(SDL_IMAGE_LIBS) $(SDL_MIXER_LIBS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -c $< -o $@

clean:
	rm $(OBJECTS) $(TARGET)

