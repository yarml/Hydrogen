

rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

# Begin configuration
MKDIR := mkdir -p
RM := rm -rf
CXX := clang++ -c -o
LINKER := clang++ -o
ANTLR := antlr4 -Dlanguage=Cpp -Xexact-output-dir -no-listener -visitor -o

INCLUDE_FLAGS := -Iinclude
CXXFLAGS := 
LINK_FLAGS := 

GRAMMARS_DIR := grammars
SRC_DIR := src

BUILD_DIR := build
# End configration

SRC_FILES := $(call rwildcard,$(SRC_DIR),*.cpp)
GRAMMAR_FILES := $(wildcard $(GRAMMARS_DIR)/*.g4)
OBJ_DIR := $(BUILD_DIR)/obj
OUTPUT := $(BUILD_DIR)/hyc
OBJ_FILES := $(patsubst %,$(OBJ_DIR)/%.o,$(SRC_FILES))

GRAMMARS_OUTPUT_DIR := $(BUILD_DIR)/grammars
GRAMMARS_OUTPUT_DIRS := $(patsubst $(GRAMMARS_DIR)/%.g4,$(GRAMMARS_OUTPUT_DIR)/%,$(GRAMMAR_FILES))

SRC_FILES += $(foreach dir,$(GRAMMARS_OUTPUT_DIRS),$(wildcard $(dir)/*.cpp))
INCLUDE_FLAGS += $(patsubst %,-I%,$(GRAMMARS_OUTPUT_DIRS))

# Begin targets
.DEFAULT_TARGET := all

.PHONY: all hyc grammars clean

all: hyc

hyc: $(OBJ_FILES) grammars
	$(info Linking output file)
	@$(LINKER) $(OUTPUT) $(LINK_FLAGS) $(OBJ_FILES)

grammars: $(GRAMMARS_OUTPUT_DIRS)

clean:
	$(info Cleaning...)
	@$(RM) build

# End targets

# Begin target templates
$(OBJ_DIR)/%.cpp.o: %.cpp
	$(info Compiling C++ source file: $<)
	@$(MKDIR) $(dir $(OBJ_DIR)/$<$(OEXT))
	@$(CXX) $(OBJ_DIR)/$<.o $(INCLUDE_FLAGS) $(CXXFLAGS) $<

$(GRAMMARS_OUTPUT_DIR)/%: $(GRAMMARS_DIR)/%.g4
	$(info Generating grammer files for: $<)
	@$(ANTLR) $@ $<
# End target templates