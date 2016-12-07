CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD
EXEC = watopoly
OBJECTS = main.o academic.o board.o gym.o player.o residences.o watopoly.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS} 

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
