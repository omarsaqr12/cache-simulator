# Cache Simulator Makefile
# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude
LDFLAGS = 

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
TEST_DATA_DIR = test_data

# Source files
SOURCES = $(SRC_DIR)/main.cpp
ALT_SOURCES = $(SRC_DIR)/cache_logic.cpp

# Executable names
TARGET = cache_simulator
ALT_TARGET = cache_logic

# Default target
all: $(TARGET) $(ALT_TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Main executable
$(TARGET): $(SOURCES) $(INCLUDE_DIR)/cache.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)

# Alternative executable (cache logic)
$(ALT_TARGET): $(ALT_SOURCES) $(INCLUDE_DIR)/cache.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(ALT_SOURCES)

# Clean build artifacts
clean:
	rm -f $(TARGET) $(ALT_TARGET) *.exe
	rm -rf $(BUILD_DIR)

# Install dependencies for GUI (requires Python)
install-deps:
	pip install ttkbootstrap

# Run tests
test: $(TARGET)
	@echo "Running cache simulator with test data..."
	@for test_file in $(TEST_DATA_DIR)/*.txt; do \
		echo "Testing with $$test_file:"; \
		echo -e "4096\n4096\n64\n2\n$$test_file" | ./$(TARGET); \
		echo ""; \
	done

# Run GUI
gui:
	python $(SRC_DIR)/gui.py

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Build both executables"
	@echo "  $(TARGET)    - Build main cache simulator"
	@echo "  $(ALT_TARGET) - Build alternative cache logic"
	@echo "  clean        - Remove build artifacts"
	@echo "  install-deps - Install Python GUI dependencies"
	@echo "  test         - Run tests with sample data"
	@echo "  gui          - Launch GUI interface"
	@echo "  help         - Show this help message"

.PHONY: all clean install-deps test gui help
