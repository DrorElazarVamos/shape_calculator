# ==============================================================================
# Robust Makefile for Vector Calculator
# Fixes "multiple definition of main" by separating App and Test builds.
# ==============================================================================

# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic \
         -ItestLIB -IcsvLIB -ImodularLIB -ImatriceLib\
		 -IvectorLIB -Iuniversal -Icalculus\
         -MMD -MP
LDFLAGS = -lm

# Build Directory
BUILD_DIR = build

# --- SOURCE CONFIGURATION ---

# 1. Directories to search
SRC_DIRS = . vectorLIB csvLIB universal modularLIB testLIB matriceLib calculus

# 2. Find ALL .c files in those directories
ALL_SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

# 3. Separate the files with 'main()' functions from the common library files
#    We filter OUT files ending in main.c, unit_test_runner.c, or system_test_run.c
LIB_SRCS := $(filter-out %main.c %unit_test_runner.c %system_test_run.c, $(ALL_SRCS))

# 4. Define Object files
#    Library Objects (Math, CSV, Logic)
LIB_OBJS := $(addprefix $(BUILD_DIR)/,$(notdir $(LIB_SRCS:.c=.o)))

#    Entry Point Objects (The 3 different main files)
MAIN_OBJ := $(BUILD_DIR)/main.o
UNIT_TEST_OBJ := $(BUILD_DIR)/unit_test_runner.o
SYSTEM_TEST_OBJ := $(BUILD_DIR)/system_test_run.o

# 5. Tell Make where to find source files
vpath %.c $(SRC_DIRS)

# --- BUILD TARGETS ---

# Default: Build everything
all: calculator unit_test system_test

# 1. The Main Calculator App
calculator: $(LIB_OBJS) $(MAIN_OBJ) | prepare_build_dir
	@echo "Linking Calculator..."
	$(CC) $(LIB_OBJS) $(MAIN_OBJ) -o calculator $(LDFLAGS)
	@echo "Build complete: calculator"

# 2. The Unit Test Runner
unit_test: $(LIB_OBJS) $(UNIT_TEST_OBJ) | prepare_build_dir
	@echo "Linking Unit Tests..."
	$(CC) $(LIB_OBJS) $(UNIT_TEST_OBJ) -o unit_test $(LDFLAGS)
	@echo "Build complete: unit_test"

# 3. The System Test Runner
system_test: $(LIB_OBJS) $(SYSTEM_TEST_OBJ) | prepare_build_dir
	@echo "Linking System Tests..."
	$(CC) $(LIB_OBJS) $(SYSTEM_TEST_OBJ) -o system_test $(LDFLAGS)
	@echo "Build complete: system_test"

# Create build directory
prepare_build_dir:
	@mkdir -p $(BUILD_DIR)

# Compile source files
$(BUILD_DIR)/%.o: %.c | prepare_build_dir
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Include dependency files
-include $(LIB_OBJS:.o=.d) $(MAIN_OBJ:.o=.d) $(UNIT_TEST_OBJ:.o=.d) $(SYSTEM_TEST_OBJ:.o=.d)

# Clean
clean:
	@echo "Cleaning artifacts..."
	rm -rf $(BUILD_DIR) calculator unit_test system_test

# Helper to run the main app
run: calculator
	./calculator

.PHONY: all clean run prepare_build_dir