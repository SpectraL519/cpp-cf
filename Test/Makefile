# TODO: os detection

# Structure
INCLUDE_DIR := Include
TEST_DIR := .
TEST_CASES_DIR := test_cases
TEST_BUILD_DIR := build
TEST_EXEC := test

# specific test case
ifeq ($(firstword $(MAKECMDGOALS)),test_case)
	TEST_CASE := $(word 2, $(MAKECMDGOALS))
	MAKECMDGOALS := $(filter-out test_case $(TEST_CASE),$(MAKECMDGOALS))
endif

# Compiler & flags
CXX := g++
CXX_STD := -std=c++2a
CXX_FLAGS += $(CXX_STD) -Wall -Wextra -Wcast-align -Wconversion -Wunreachable-code -Wuninitialized -pedantic -g -O3
COMPILE_ARGS := -I $(INCLUDE_DIR) $(CXX_FLAGS)

# Test source & object files
TEST_SOURCES := $(wildcard $(TEST_CASES_DIR)/*.cpp)
TEST_OBJECTS := $(patsubst $(TEST_CASES_DIR)/%.cpp, $(TEST_BUILD_DIR)/%.o, $(TEST_SOURCES))

TEST_SOURCES += $(TEST_DIR)/main.cpp
TEST_OBJECTS += $(TEST_BUILD_DIR)/main.o



test:
$(TEST_BUILD_DIR)/main.o: main.cpp
	@echo Compiling: $<
	@$(CXX) -c $< -o $@ $(COMPILE_ARGS)

$(TEST_BUILD_DIR)/%.o: $(TEST_CASES_DIR)/%.cpp
	@echo Compiling: $<
	@$(CXX) -c $< -o $@ $(COMPILE_ARGS)

$(TEST_EXEC): $(TEST_OBJECTS)
	@echo Linking: $@
	@$(CXX) $^ -o $@ $(COMPILE_ARGS)
	@echo Build successful!



test_case:
	@if exist "$(TEST_CASES_DIR)\$(TEST_CASE).cpp" (
		@echo Building test: $(TEST_CASE)
		@echo Compiling: $(TEST_DIR)/main.cpp
		@$(CXX) -c "$(TEST_DIR)/main.cpp" -o "$(TEST_BUILD_DIR)/main.o" $(COMPILE_ARGS)
		@echo Compiling: $(TEST_CASES_DIR)\$(TEST_CASE).cpp
		@$(CXX) -c "$(TEST_CASES_DIR)\$(TEST_CASE).cpp" -o "$(TEST_BUILD_DIR)/$(TEST_CASE).o" $(COMPILE_ARGS)
		@echo Linking...
		@$(CXX) $(COMPILE_ARGS) "$(TEST_BUILD_DIR)/main.o" "$(TEST_BUILD_DIR)/$(TEST_CASE).o" -o "$(TEST_CASE)"
		@echo Build successful!
	) else (
		# TODO: fix the else error msg showing
		@echo Test not found: $(TEST_CASE)
	)




clean:
	del /Q $(TEST_BUILD_DIR)\*.o $(TEST_DIR)\*.exe
