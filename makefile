#LDFLAGS = -lSDL2 -lSDL2_image -lrt -lasound -ljack -lpthread -lportaudio -lm
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
INCLUDES = -I/usr/include/SDL2 -Iinc/ -Idanklib/ -I.
CFLAGS = -Wall -Werror -fsanitize=address -Wfatal-errors -g -O3

# Warning exceptions
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-const-variable
CFLAGS += -Wno-unused-but-set-variable # might be a useful actually
CFLAGS += -Wno-narrowing # might be useful too, we will c
CFLAGS += -Wno-sign-compare
#CFLAGS += -Wno-unknown-warning-option # so we can compile with both lol

CFLAGS += -std=c++20

SRCS = $(wildcard *.c)
SRCS += $(wildcard *.cpp)
SRCS += $(wildcard danklib/*.c)
SRCS += $(wildcard danklib/*.cpp)


snowkoban: $(SRCS)
	g++ $(SRCS) -o  snowkoban $(CFLAGS) $(INCLUDES) $(LDFLAGS)

.PHONY: clean
clean:
