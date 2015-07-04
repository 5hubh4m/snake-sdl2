CC = gcc
CXX = g++
CXXFLAGS = -std=c++11 -Wall
INCLUDES =
LFLAGS =
LIBS = -lSDL2 -lSDL2_ttf -lSDL2_image
SRCS = main.cpp texture.cpp snake.cpp score.cpp snake_sdl2.cpp
OBJS = $(SRCS:.cpp=.o)
MAIN = snake_sdl2

.PHONY : depend clean

all : $(MAIN)
	@echo snake_sdl2 compiled and linked

$(MAIN) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<  -o $@

clean :
	$(RM) *.o *~ $(MAIN) *db

run :
	./$(MAIN)

depend : $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
