CC = gcc
CFLAGS = -Wall -Wextra -O2 -fPIC -Iinclude
LDFLAGS = -lm

BUILD_DIR = build
LIB_TARGET = $(BUILD_DIR)/libfinanzas.so
TEST_TARGET = $(BUILD_DIR)/test_finanzas

.PHONY: all clean test

all: $(LIB_TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIB_TARGET): src/finanzas.c include/finanzas.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -shared src/finanzas.c -o $(LIB_TARGET) $(LDFLAGS)

$(TEST_TARGET): tests/test_finanzas.c $(LIB_TARGET)
	$(CC) -Wall -Wextra -O2 -Iinclude tests/test_finanzas.c -L$(BUILD_DIR) -lfinanzas $(LDFLAGS) -o $(TEST_TARGET)

test: $(TEST_TARGET)
	LD_LIBRARY_PATH=$(BUILD_DIR) ./$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR)
