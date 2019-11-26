CC = gcc
LD = gcc

CCFLAGS = -std=c11 -D_XOPEN_SOURCE=500
CCFLAGS_DEBUG = $(CCFLAGS) -O0 -g
CCFLAGS_OPT = $(CCFLAGS) -O2
CCFLAGS_TSAN = $(CCFLAGS) -fsanitize=thread -O2 -g

LDFLAGS = -lrt -lpthread -lm
LDFLAGS_TSAN = $(LDFLAGS) -fsanitize=thread

OBJS_DIR = .objs
OBJS = shmemq.o main.o
TARGET = shmemq

all: $(TARGET)

debug: clean $(TARGET)-debug

tsan: clean $(TARGET)-tsan

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%-debug.o: %.c | $(OBJS_DIR)
	$(CC) -c $< -o $@ $(CCFLAGS_DEBUG)

$(OBJS_DIR)/%-tsan.o: %.c | $(OBJS_DIR)
	$(CC) -c $< -o $@ $(CCFLAGS_TSAN)

$(OBJS_DIR)/%-opt.o: %.c | $(OBJS_DIR)
	$(CC) -c $< -o $@ $(CCFLAGS_OPT)

$(TARGET)-debug: $(OBJS:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ -o $@ $(LDFLAGS)

$(TARGET)-tsan: $(OBJS:%.o=$(OBJS_DIR)/%-tsan.o)
	$(LD) $^ -o $@ $(LDFLAGS_TSAN)

$(TARGET): $(OBJS:%.o=$(OBJS_DIR)/%-opt.o)
	$(LD) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(OBJS_DIR) $(TARGET) $(TARGET)-debug
