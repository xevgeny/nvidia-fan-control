CFLAGS = -Wall -Wextra -Wpedantic -std=c23
LDFLAGS = -lnvidia-ml -lconfig

SRCS = nvidia-fan-control.c config.c util.c
OBJS = $(BUILD_DIR)/nvidia-fan-control.o $(BUILD_DIR)/config.o $(BUILD_DIR)/util.o
TEST_OBJS = $(BUILD_DIR)/test.o $(BUILD_DIR)/config.o $(BUILD_DIR)/util.o
BUILD_DIR = build
TARGET = nvidia-fan-control
TEST_TARGET = test

.PHONY: all clean test

all: $(BUILD_DIR)/$(TARGET)

test: $(BUILD_DIR)/$(TEST_TARGET)
	./$(BUILD_DIR)/$(TEST_TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/$(TEST_TARGET): $(TEST_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(TEST_OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -I./include -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)