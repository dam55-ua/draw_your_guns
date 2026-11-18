# Makefile con regla build y dependencias automáticas (-MMD -MP)
# Compila por ficheros: cada .cpp genera su .o y su .d en build/

TARGET := game
BUILD := build
SRC_DIR := src
CXX := ccache g++
CXXFLAGS := -Wall -Wextra -MMD -MP
SRCS := $(shell find $(SRC_DIR) -type f -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)
INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS)) -I vendor/include
LIB_FLAGS := -L vendor/lib
LDLIBS := -lraylib -lGL -lm -lpthread -lrt -lX11

.PHONY: all build clean

all: build

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LIB_FLAGS) $(LDLIBS)

$(BUILD)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) -c -o $@ $<

-include $(DEPS)

clean:
	$(RM) $(TARGET)
	$(RM) -r $(BUILD)
