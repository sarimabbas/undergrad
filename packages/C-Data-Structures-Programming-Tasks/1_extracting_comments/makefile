# This makefile was adapted from Prof. Aspnes notes

# These lines set variables that control make's default rules.
# We STRONGLY recommend putting "-Wall -pedantic -g3" in your CFLAGS.
CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

# The next line is a dependency line.
# It says that if somebody types "make all"
# make must first make "hello-world".
# By default the left-hand-side of the first dependency is what you
# get if you just type "make" with no arguments.
all: Comments

# How do we make Comments?
# The dependency line says you need to first make comments.o
Comments: comments.o
# Subsequent lines starting with a TAB character give
# commands to execute.
# This command uses make built-in variables to avoid
# retyping (and getting things wrong):
#  $@ = target hello-world
#  $^ = dependencies hello-world.o and hello-library.o
	$(CC) $(CFLAGS) -o $@ $^
# You can put whatever commands you want.

# Here we are saying that comments.o
# should be rebuilt whenever their corresponding source file changes.
# There are no commands attached to these dependency lines, so
# make will have to figure out how to do that somewhere else
# (probably from the builtin .c -> .o rule).
comments.o: comments.c

# This lets you type "make clean" and get rid of anything you can
# rebuild.  The $(RM) variable is predefined to "rm -f"
clean:
	$(RM) comments *.o
