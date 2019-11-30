CC = g++
CCFLAGS = -Wall -g -std=c++11
PREFLAGS = -Wall -c -std=c++11
INCLUDE = -I /Users/durand/Desktop/Centre/A\ propos\ de\ programmes/CPP/breakout/include
LIBS = -L /Users/durand/Desktop/Centre/A\ propos\ de\ programmes/CPP/breakout/lib-macos -lglfw3 -lfreetype -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
DEPENDENCIESCPP = main.cpp game.cpp shader.cpp stb_image.cpp texture.cpp resource_manager.cpp sprite.cpp object.cpp level.cpp ball.cpp glad.cpp particle.cpp text_renderer.cpp
DEPS = *.h
AUX = *.o

%.o: %.cpp $(DEPS)
	$(CC) $(INCLUDE) $(PREFLAGS) -o $@ $<



main: $(AUX)
	$(CC) $(CCFLAGS) $(INCLUDE) -o $@ $^ $(LIBS)
