CC = gcc
LD = gcc

CCFLAGS = -std=c11 -O3
LDFLAGS = -lrt

OBJS_DIR = .objs
OBJS = shmemq.o main.o
TARGET = shmemq

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: %.c | $(OBJS_DIR)
	$(CC) -c $< -o $@ $(CCFLAGS)

$(TARGET): $(OBJS:%.o=$(OBJS_DIR)/%.o)
	$(LD) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(OBJS_DIR) $(TARGET)
