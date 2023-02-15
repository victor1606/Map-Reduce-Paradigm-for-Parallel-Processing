GCC = g++
CFLAGS = -o tema1 -Wall -Wextra -lpthread
RM = rm -rf

build:
	$(GCC) main.cpp parallel.cpp helpers.cpp $(CFLAGS)

clean:
	$(RM) ./tema1
	