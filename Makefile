CC = g++

ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
CFLAGS = -g -DGL_GLEXT_PROTOTYPES -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED -DOSX -Wno-deprecated-register -Wno-deprecated-declarations -Wno-shift-op-parentheses -Wno-parentheses-equality
INCFLAGS = -I./glm-0.9.7.1 -I./glfw-3.1.2/ -I/usr/X11/include -I./include/
LDFLAGS = -framework GLUT -framework OpenGL -lglfw -framework Cocoa -framework IOKit -framework CoreVideo -L./lib/osx/ \
		-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
		-lGL -lGLU -lm -lstdc++ -lfreeimage
else
CFLAGS = -g -DGL_GLEXT_PROTOTYPES
INCFLAGS = -I./glm-0.9.7.1 -I./glfw-3.1.2/ -I./include/ -I/usr/X11R6/include -I/sw/include \
		-I/usr/sww/include -I/usr/sww/pkg/Mesa/include
LDFLAGS = -L./lib/nix -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib -Lglfw3/lib/ \
		-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lGLEW -lglut -lglfw -framework Cocoa -framework IOKit -framework CoreVideo -lGLU -lGL -lX11 -lfreeimage
endif

RM = /bin/rm -f 
all: mesh_viewer

mesh_viewer: main.o
	$(CC) $(CFLAGS) -o mesh_viewer main.o $(INCFLAGS) $(LDFLAGS) 
main.o: main.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp $(INCFLAGS) $(LDFLAGS) 
clean: 
	$(RM) *.o mesh_viewer *.png