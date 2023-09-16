PROJECT:=calculator

CC:=gcc
LDFLAGS=-Wall -Wextra -Wpedantic -Werror -O2 -ggdb
#LDFLAGS=-Wall -Wextra -Wpedantic -Og -g3 -ggdb -fsanitize=address,undefined
CXXFLAGS=-std=c99 -Iinclude
UNAME:=$(shell uname)

SRCDIR:=src
OBJDIR:=obj

SRC:=$(wildcard $(SRCDIR)/*.c)
OBJ:=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(PROJECT): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CXXFLAGS) -c $< -o $@
	
$(OBJDIR):
	mkdir $(OBJDIR)

.PHONY: all
all: clean $(PROJECT)

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(PROJECT)
