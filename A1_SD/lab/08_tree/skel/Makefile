CC := gcc
CFLAGS := -std=c99 -g -Wall -Wextra

TASKS := task1 task2 bonus
SRCDIR := src

ALL_SRCS := $(wildcard $(SRCDIR)/*.c)
SRCFILES := $(filter-out $(SRCDIR)/task%.c, $(ALL_SRCS))
SRCFILES := $(filter-out $(SRCDIR)/bonus.c, $(SRCFILES))
OBJFILES := $(patsubst $(SRCDIR)/%.c,%.o,$(SRCFILES))

.PHONY: build clean

build: task1 task2 bonus

run: task1
	./task1

run_task2: task2
	./task2

task1: $(OBJFILES) $(SRCDIR)/task1.c
	$(CC) $(CFLAGS) -o $@ $^

task2: $(OBJFILES) $(SRCDIR)/task2.c
	$(CC) $(CFLAGS) -o $@ $^

bonus: $(OBJFILES) $(SRCDIR)/bonus.c
	$(CC) $(CFLAGS) -o $@ $^

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJFILES) $(TASKS)
