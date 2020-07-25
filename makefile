SRC_DIR := src
OBJ_DIR := obj
LIB_DIR := lib

EXE := audio_lib.so
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CPPFLAGS := -Iinclude -MMD -MP -fPIC
CFLAGS   := -Wall -std=c++17
LDFLAGS  := -shared
LDLIBS   :=

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(LIB_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $(LIB_DIR)/$@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIB_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(LIB_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)