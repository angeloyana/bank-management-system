SRCDIR = src
INCDIR = include
BUILDDIR = build
TARGET = $(BUILDDIR)/bms

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))

CC = gcc
CFLAGS = -I$(INCDIR) -g -O0 -Wall -Wextra -std=c11

$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILDDIR)
	$(CC) -o $(TARGET) $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf build

.PHONY: format
format:
	clang-format -style=Google -i $(SOURCES) $(wildcard $(INCDIR)/*.h)
