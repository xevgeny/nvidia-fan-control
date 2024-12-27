# Build configuration
CFLAGS = -Wall -Wextra -Wpedantic -std=c23
LDFLAGS = -lnvidia-ml -lconfig

# Installation paths (can be overridden)
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
	@if [ "$$(id -u)" != "0" ]; then \
		echo "Please run as root or with sudo"; \
		exit 1; \
	fi
	# Create directories
	install -d $(BINDIR)
	install -d $(CONFDIR)
	# Install binary
	install -m 755 $(BUILD_DIR)/$(TARGET) $(BINDIR)/
	# Install config if it doesn't exist
	if [ ! -f $(CONFDIR)/config.cfg ]; then \
		install -m 644 example.cfg $(CONFDIR)/config.cfg; \
		echo "Installed default config to $(CONFDIR)/config.cfg"; \
	else \
		echo "Config file already exists, skipping..."; \
	fi
	@echo "Installation complete!"
	@echo "Binary installed to: $(BINDIR)/$(TARGET)"
	@echo "Config location: $(CONFDIR)/config.cfg"

uninstall:
	@if [ "$$(id -u)" != "0" ]; then \
		echo "Please run as root or with sudo"; \
		exit 1; \
	fi
	rm -f $(BINDIR)/$(TARGET)
	rm -rf $(CONFDIR)
	@echo "Uninstallation complete!"

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