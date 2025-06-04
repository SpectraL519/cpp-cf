CXX       := g++
CXXFLAGS  := -std=c++20 -Wall -Werror -I ../include
BUILD_DIR := build

TARGETS := adder accumulator str_joiner
BINARIES := $(addprefix $(BUILD_DIR)/, $(TARGETS))

all: $(BINARIES)

$(BUILD_DIR)/%: %.cpp | $(BUILD_DIR)
	$(CXX) -o $@ $< $(CXXFLAGS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
