CXX = g++
CXXFLAGS = -Werror -MMD -O2 -lmingw32 -lSDL2main -lSDL2
EXEC = allrgb
SOURCES = $(wildcard *.cc)
OBJECTS = $(SOURCES:.cc=.o)
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} ${CXXFLAGS}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
