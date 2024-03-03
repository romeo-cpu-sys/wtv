CC := gcc
CFLAGS := -Wall -Wextra -std=c11
LDFLAGS := -lcrypto

SRC_DIR := src
INCLUDE_DIR := includes
BUILD_DIR := build

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
DEP_FILES := $(OBJ_FILES:.o=.d)

EXECUTABLE := my_program

.PHONY: all clean

all: $(BUILD_DIR) $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -MMD -MP -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)

-include $(DEP_FILES)
