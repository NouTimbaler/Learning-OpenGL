CXX=g++
CXXFLAGS=-Wall -std=c++17
LIBS=-lGL -lglfw -lGLEW
CPPFLAGS= $(INC_FLAGS) $(LOCURA_FLAGS)


SRC_DIR := ./src
BUILD_DIR := ./build

# Find all cpp files
SRCS := $(notdir $(shell find $(SRC_DIR) -name *.cpp))

# Create obj names
OBJ := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# Locura que he trobat a internet
DEPS := $(OBJS:.o=.d)
LOCURA_FLAGS := -MMD -MP

# Find header dirs
INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))


OpenGLapp: $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@



.PHONY: clean
clean:
	rm -rf OpenGLapp $(BUILD_DIR)


# aixo es part de la locura
-include $(DEPS)
