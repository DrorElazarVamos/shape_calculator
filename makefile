# ==============================================================================
# Makefile for Vector & Volume Calculator
# Author: Dror Elazar Vamos
#
# FINAL VERSION: Routes all object files (.o) into the dedicated 'build/' folder.
# FIX: Structural change to resolve 'missing separator' errors on main targets.
# ==============================================================================

# Compiler and flags
CC = gcc
# CFLAGS: Header search paths for all subdirectories.
CFLAGS = -Wall -Wextra -std=c99 -pedantic -ItestLIB -IcsvLIB -ImodularLIB -IbitOperation -IvectorLIB -IlimitsLib
LDFLAGS = -lm
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O2

# Target executable
TARGET = calculator
# NEW: Directory for all object files
BUILD_DIR = build

# Source directories for VPATH (tells 'make' where to look for source/header files)
VPATH = testLIB:csvLIB:modularLIB:bitOperation:vectorLIB.

# Source files (just the filenames)
SOURCES = main.c \
          testerFile.c \
          csvHandler.c \
          modular.c \
          bitOperation.c \
          vectorOps.c 

# UPDATED: Object files now include the path to the build directory (e.g., build/main.o)
OBJECTS = $(addprefix $(BUILD_DIR)/, $(SOURCES:.c=.o))

# Default target
all: $(TARGET)

# Build the executable (Depends on all object files and the preparation target)
$(TARGET): $(OBJECTS) | prepare_build_dir
	@echo "Linking $(TARGET)..."
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

# TARGET TO CREATE BUILD DIRECTORY (Uses order-only prerequisite '|' above)
prepare_build_dir:
	@mkdir -p $(BUILD_DIR)

# Compile source files to object files (Pattern Rule)
# The target $@ is now $(BUILD_DIR)/filename.o (e.g., build/main.o)
$(BUILD_DIR)/%.o: %.c | prepare_build_dir
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean prepare_build_dir $(TARGET)
	@echo "Debug build complete"

# Release build (optimized)
release: CFLAGS += $(RELEASE_FLAGS)
release: clean prepare_build_dir $(TARGET)
	@echo "Release build complete"

# Run the program
run: $(TARGET)
	@echo "Running $(TARGET)..."
	./$(TARGET)

# Run with test file
test: $(TARGET)
	@echo "Running automated tests..."
	@if [ -f comprehensive_test_cases.csv ]; then \
		./$(TARGET); \
	else \
		echo "Error: comprehensive_test_cases.csv not found"; \
	fi

check:
	@echo "Checking for required files..."
	@for file in $(SOURCES); do \
		if [ ! -f $$file ] && \
		   [ ! -f testLIB/$$file ] && \
		   [ ! -f csvLIB/$$file ] && \
		   [ ! -f modularLIB/$$file ] && \
		   [ ! -f bitOperation/$$file ] && \
		   [ ! -f vectorLIB/$$file ] && \
		   [ ! -f limitsLib/$$file ] && \  <-- ADD THIS LINE
		   [ ! -f ./$$file ]; then \
			echo "Warning: $$file not found in any expected directory"; \
		fi; \
	done
	@echo "Check complete"

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR) $(TARGET)
	@echo "Clean complete"

# Clean everything including backup files
distclean: clean
	@echo "Removing backup and temporary files..."
	rm -f *~ *.bak *.swp core

# Install (copy to /usr/local/bin - requires sudo)
install: $(TARGET)
	@echo "Installing $(TARGET)..."
	install -m 0755 $(TARGET) /usr/local/bin
	@echo "Installation complete"

# Uninstall
uninstall:
	@echo "Uninstalling $(TARGET)..."
	rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstall complete"

# Show help
help:
	@echo "Available targets:"
	@echo "  all       - Build the project (default)"
	@echo "  debug     - Build with debug symbols"
	@echo "  release   - Build optimized release version"
	@echo "  run       - Build and run the program"
	@echo "  test      - Run automated test suite"
	@echo "  clean     - Remove object files and executable"
	@echo "  distclean - Remove all generated files"
	@echo "  install   - Install to /usr/local/bin (requires sudo)"
	@echo "  uninstall - Remove from /usr/local/bin"
	@echo "  help      - Show this help message"

# Check for missing source files
check:
	@echo "Checking for required files..."
	@for file in $(SOURCES); do \
		if [ ! -f $$file ] && [ ! -f testLIB/$$file ] && [ ! -f csvLIB/$$file ] && [ ! -f modularLIB/$$file ] && [ ! -f bitOperation/$$file ] && [ ! -f vectorLIB/$$file ] && [ ! -f ./$$file ]; then \
			echo "Warning: $$file not found in any expected directory"; \
		fi; \
	done
	@echo "Check complete"

# Phony targets (not actual files)
.PHONY: all debug release run test clean distclean install uninstall help check prepare_build_dir