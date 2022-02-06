rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

# Begin configuration
MKDIR := mkdir -p
RM := rm -rf
CD := cd
CP := cp
TOUCH := touch
CXX := clang++ -g -ferror-limit=1 -std=c++20 -c -o
LINKER := clang++ -g -ferror-limit=1 -std=c++20 -o

INCLUDE_FLAGS := -Iinclude -Ilibs/include
CXXFLAGS := 
LINK_FLAGS :=

SRC_DIR := src

EXAMPLES := examples

BUILD_DIR := build

HYC_FLAGS := -d -ll debug -o test input.hy
# End configration

SRC_FILES := $(call rwildcard,$(SRC_DIR),*.cpp)
OBJ_DIR := $(BUILD_DIR)/obj
OUTPUT := $(BUILD_DIR)/hyc

OBJ_FILES := $(patsubst %,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Begin targets
.DEFAULT_TARGET := all

.PHONY: all hyc clean

all: hyc

hyc: $(OBJ_FILES)
	$(info Linking output file)
	@$(LINKER) $(OUTPUT) $(LINK_FLAGS) $(OBJ_FILES)

clean:
	$(info Cleaning...)
	@$(RM) $(BUILD_DIR)

run: hyc input
	@$(CD) $(BUILD_DIR) && ./$(notdir $(OUTPUT)) $(HYC_FLAGS)

input: $(BUILD_DIR)/input.hy
	
# End targets
$(BUILD_DIR)/input.hy: $(EXAMPLES)/input.hy
	@$(CP) $(EXAMPLES)/input.hy $(BUILD_DIR)

# Begin target templates
$(OBJ_DIR)/%.cpp.o: %.cpp
	$(info Compiling C++ source file: $<)
	@$(MKDIR) $(dir $(OBJ_DIR)/$<$(OEXT))
	@$(CXX) $(OBJ_DIR)/$<.o $(INCLUDE_FLAGS) $(CXXFLAGS) $<

# End target templates