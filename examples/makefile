SRC := $(wildcard *.cpp)
TEST := $(SRC:%.cpp=%)


CPPFLAGS := -I../include
CFLAGS   := -Wall -std=c++17
LDFLAGS  :=
LDLIBS   := ../lib/audio_lib.so

.PHONY: all clean

all: $(TEST)

%: %.cpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) $< $(LDLIBS) -o $@

clean:
	@$(RM) -rv $(TEST)