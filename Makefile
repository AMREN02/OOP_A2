CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
SOURCES = main.cpp UI.cpp VoleMachine.cpp CPU.cpp Memory.cpp RegisterFile.cpp Screen.cpp Instruction.cpp InstructionFactory.cpp ProgramLoader.cpp FloatConverter.cpp
TARGET = vole

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
