# Target name
TARGET = main

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall

# Source files (ensure these match the actual filenames)
SRCS = main.cpp Reader.cpp Tokenizer.cpp Types.cpp core.cpp printer.cpp

# Object files (same as source files but with .o extension)
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link the object files to create the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) -g

# Compile each .cpp file into a .o file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

ifeq ($(OS),Windows_NT)
    CLEAN_CMD = del /q
else
    CLEAN_CMD = rm -f
endif

# Clean up build files
clean:
	$(CLEAN_CMD) $(OBJS) $(TARGET)
