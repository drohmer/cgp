# This Makefile will generate an executable file named example_compilation

# This path should point to the CGP library depending on the current directory
## You may need to it in case you move the position of your directory
PATH_TO_CGP = ../../library/

TARGET ?= example_compilation #name of the executable
SRC_DIRS ?= src/ $(PATH_TO_CGP)
CXX = g++ #Or clang++

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS  := . $(PATH_TO_CGP)
INC_FLAGS := $(addprefix -I,$(INC_DIRS)) $(shell pkg-config --cflags glfw3)

CPPFLAGS += $(INC_FLAGS) -MMD -MP -DIMGUI_IMPL_OPENGL_LOADER_GLAD -g -O2 -std=c++14 -Wall -Wextra -Wfatal-errors -Wno-sign-compare -Wno-type-limits -Wno-pragmas -DSOLUTION # Adapt these flags to your needs

LDLIBS += $(shell pkg-config --libs glfw3) -ldl -lm # Adapt this lib depending on your system (lib glfw is usually at -lglfw)

$(TARGET): $(OBJS)
	echo $(CURDIR)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS) imgui.ini

-include $(DEPS)