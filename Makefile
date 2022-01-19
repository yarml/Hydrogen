# TODO: make it so simply executing `make` builds without failures
# to do that we need to update OBJ_FILES for each grammar we build

# Also make it so that HydrogenLexer grammar being generated before HydrogenParser is not just a fortunate accident

rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

# Begin configuration
MKDIR := mkdir -p
RM := rm -rf
TOUCH := touch
CXX := clang++ -g -ferror-limit=1 -std=c++20 -c -o
LINKER := clang++ -g -ferror-limit=1 -std=c++20 -o
ANTLR := antlr4 -Dlanguage=Cpp -Xexact-output-dir -no-listener -no-visitor -o

INCLUDE_FLAGS := -Iinclude -Ilibs/include -I/usr/include/antlr4-runtime
CXXFLAGS := 
LINK_FLAGS := -lantlr4-runtime

GRAMMARS_DIR := grammars
SRC_DIR := src

BUILD_DIR := build
# End configration

SRC_FILES := $(call rwildcard,$(SRC_DIR),*.cpp)
GRAMMAR_FILES := $(wildcard $(GRAMMARS_DIR)/*.g4)
OBJ_DIR := $(BUILD_DIR)/obj
OUTPUT := $(BUILD_DIR)/hyc

GRAMMARS_OUTPUT_DIR := $(BUILD_DIR)/grammars
GRAMMARS := $(patsubst $(GRAMMARS_DIR)/%.g4,$(GRAMMARS_OUTPUT_DIR)/%.build,$(GRAMMAR_FILES))

SRC_FILES += $(wildcard $(GRAMMARS_OUTPUT_DIR)/*.cpp)
INCLUDE_FLAGS += -I$(GRAMMARS_OUTPUT_DIR)

OBJ_FILES := $(patsubst %,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Begin targets
.DEFAULT_TARGET := all

.PHONY: all hyc grammars clean

all: hyc

hyc: grammars $(OBJ_FILES)
	$(info Linking output file)
	@$(LINKER) $(OUTPUT) $(LINK_FLAGS) $(OBJ_FILES)

grammars: $(GRAMMARS)

clean:
	$(info Cleaning...)
	@$(RM) build

# End targets

# Begin target templates
$(OBJ_DIR)/%.cpp.o: %.cpp
	$(info Compiling C++ source file: $<)
	@$(MKDIR) $(dir $(OBJ_DIR)/$<$(OEXT))
	@$(CXX) $(OBJ_DIR)/$<.o $(INCLUDE_FLAGS) $(CXXFLAGS) $<

$(GRAMMARS_OUTPUT_DIR)/%.build: $(GRAMMARS_DIR)/%.g4
	$(info Generating grammer files for: $<)
	@$(ANTLR) $(GRAMMARS_OUTPUT_DIR) $<
	@$(TOUCH) $@
# End target templates