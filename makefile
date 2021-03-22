# -j4

#LDFLAGS = -lSDL2 -lSDL2_image -lrt -lasound -ljack -lpthread -lportaudio -lm
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm  -fsanitize=address
INCLUDES = -I/usr/include/SDL2 -Iinc/ -Idanklib/ -I.
CFLAGS = -Wall -Werror -Wfatal-errors -g -O3 -MMD -pipe

# Warning exceptions
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-const-variable
CFLAGS += -Wno-unused-but-set-variable # might be a useful actually
CFLAGS += -Wno-narrowing # might be useful too, we will c
CFLAGS += -Wno-sign-compare

CFLAGS += -std=c++20

SRCS += $(wildcard *.cpp)
SRCS += $(wildcard danklib/*.cpp)

ODIR := crap

OBJ := $(patsubst %.cpp, $(ODIR)/%.o, ${SRCS})
#OBJ = $(SRCS:.cpp=.o)

CC = g++


.PHONY: all clean

all: dirs snowkoban

dirs:
	mkdir -p $(ODIR)/danklib

run: all
	./snowkoban

snowkoban: $(OBJ)
	$(CC) -o  snowkoban $^ $(LDFLAGS)

$(ODIR)/%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES)

-include $(OBJ:.o=.d)

clean: all
	rm -f snowkoban $(ODIR)/*.o $(ODIR)/*.d