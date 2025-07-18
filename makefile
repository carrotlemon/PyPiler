# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++20 -g

# Source files
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)

# Output binary
TARGET := pypiler

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compilation
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: all
	./$(TARGET)

.PHONY: all clean run