CXX = g++
CXXFLAGS = -Wall -MMD -lmingw32 -lSDL2main -lSDL2 -std=c++11 -DDEBUG=0
EXEC = allrgb
OBJECTS = main.o sdlengine.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} ${CXXFLAGS}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
