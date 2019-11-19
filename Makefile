CC = gcc
LD = gcc

OBJS_DIR = .objs
OBJS = shmemq.o main.o
TARGET = shmemq

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: %.c | $(OBJS_DIR)
	$(CC) -c $< -o $@

$(TARGET): $(OBJS:%.o=$(OBJS_DIR)/%.o)
	$(LD) $^ -o $@
