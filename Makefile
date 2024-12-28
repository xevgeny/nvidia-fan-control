# Build configuration
CFLAGS = -Wall -Wextra -Wpedantic -std=c23
LDFLAGS = -lnvidia-ml -lconfig

# Installation paths
PREFIX ?= /usr/local
SYSCONFDIR ?= /etc
BINDIR = $(PREFIX)/bin
CONFDIR = $(SYSCONFDIR)/nvidia-fan-control

# Source files
SRCS = nvidia-fan-control.c config.c util.c
OBJS = $(BUILD_DIR)/nvidia-fan-control.o $(BUILD_DIR)/config.o $(BUILD_DIR)/util.o
TEST_OBJS = $(BUILD_DIR)/test.o $(BUILD_DIR)/config.o $(BUILD_DIR)/util.o
BUILD_DIR = build
TARGET = nvidia-fan-control
TEST_TARGET = test

.PHONY: all clean test install uninstall

all: $(BUILD_DIR)/$(TARGET)

test: $(BUILD_DIR)/$(TEST_TARGET)
	./$(BUILD_DIR)/$(TEST_TARGET)

install: all
	install -d $(BINDIR)
	install -d $(CONFDIR)
	install -m 755 $(BUILD_DIR)/$(TARGET) $(BINDIR)/
	if [ ! -f $(CONFDIR)/config.cfg ]; then \
		install -m 644 example.cfg $(CONFDIR)/config.cfg; \
		echo "Installed default config to $(CONFDIR)/config.cfg"; \
	else \
		echo "Config file already exists, skipping..."; \
	fi
	@echo "Successfully installed $(TARGET) to $(BINDIR)/$(TARGET)"

uninstall:
	rm -f $(BINDIR)/$(TARGET)
	rm -rf $(CONFDIR)
	@echo "Successfully uninstalled $(TARGET)"

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