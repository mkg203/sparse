CC = gcc
CFLAGS = -Iinclude

SRCDIR = src
BUILDDIR = build

SRC = $(SRCDIR)/main.c $(SRCDIR)/handler.c $(SRCDIR)/arg.c
OBJ = $(SRC:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

program: $(OBJ)
	$(CC) $(OBJ) -o main

# Rule to compile .c into build/*.o
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) main
