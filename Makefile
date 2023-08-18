PROJECT:=calculator

CC:=gcc
LDFLAGS=-O2 -Iinclude -std=c99 -Wall -ggdb
UNAME:=$(shell uname)

SRCDIR:=src
OBJDIR:=obj

SRC:=$(wildcard $(SRCDIR)/*.c)
OBJ:=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(PROJECT): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(LDFLAGS) -c $< -o $@
	
$(OBJDIR):
	mkdir $(OBJDIR)

.PHONY: all
all: clean $(PROJECT)

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(PROJECT)
