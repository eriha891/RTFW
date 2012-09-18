OS = $(shell uname -s)
CC = g++
MV = mv
CP = cp
RM = rm
MESSAGE =
OPENGL=1

#specify dependency list
OBJECTS ?= src/AABB.o src/GeometricTools.o src/ObjLoader.o src/SimpleRenderer.o src/main.o
OUTPUT ?= Program
FLAGS ?= 
INCPATH ?=

# check if argument OPENGL=1 is set, reguires glfw to be properly installed
ifdef OPENGL
	MESSAGE += OpenGL,
	ifeq ($(OS),Linux)
		# CPPFLAGS := -lGL -lGLU -lX11 -static-libgcc -static-libstdc++
		FLAGS += -lglfw
	else ifeq ($(OS),Darwin)
		FLAGS += -framework Cocoa -framework OpenGL -lglfw
	endif
endif



all: $(OBJECTS)
	-@echo "Linking for $(OS)"
	-@echo "Including $(MESSAGE)"
	-@echo " "
	$(CC) -o $(OUTPUT) $(OBJECTS) $(FLAGS) $(INCPATH)

clean:
	-@echo "Cleaning"
	-$(RM) *.o

# pattern that compiles all .o files, both .cpp and .c works (tested on linux)
%.o: %.c
	$(CC) $(INCPATH) -c $< -o $@

