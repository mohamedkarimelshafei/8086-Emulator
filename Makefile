CXX = clang++
CXXFLAGS = -std=c++17 -Wall -O2

SRC = \
main.cpp \
cpu.cpp \
decoder.cpp \
modrm.cpp \
flags.cpp \
stack.cpp \
instructions_mov.cpp \
instructions_add.cpp \
instructions_sub.cpp \
instructions_cmp.cpp

OBJ = $(SRC:.cpp=.o)

emu: $(OBJ)
	$(CXX) $(OBJ) -o emu

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o emu
