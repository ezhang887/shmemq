CC = gcc
LD = gcc

CCFLAGS = -std=c11 -D_XOPEN_SOURCE=500
CCFLAGS_DEBUG = $(CCFLAGS) -O0 -g
CCFLAGS_OPT = $(CCFLAGS) -O3

LDFLAGS = -lrt -lpthread -lm

OBJS_DIR = .objs
OBJS = shmemq.o main.o
TARGET = shmemq

all: $(TARGET)

debug: clean $(TARGET)-debug

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%-debug.o: %.c | $(OBJS_DIR)
	$(CC) -c $< -o $@ $(CCFLAGS_DEBUG)

$(OBJS_DIR)/%-opt.o: %.c | $(OBJS_DIR)
	$(CC) -c $< -o $@ $(CCFLAGS_OPT)

$(TARGET)-debug: $(OBJS:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ -o $@ $(LDFLAGS)

$(TARGET): $(OBJS:%.o=$(OBJS_DIR)/%-opt.o)
	$(LD) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(OBJS_DIR) $(TARGET) $(TARGET)-debug
