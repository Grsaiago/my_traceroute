NAME = my_traceroute
TEST_BIN = test_runner

# needed directories
SRC_DIR = src
OBJ_DIR = obj
COVER_DIR = coverage
TEST_DIR = tests

# files used for coverage
RAW_PROFILE_FILE = my_traceroute.profraw
PROFILE_FILE = my_traceourte.profdata

# main object/source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(filter-out $(OBJ_DIR)/main.o, $(OBJS))

CC = clang

# coverage docs: https://llvm.org/docs/CommandGuide/llvm-cov.html#show-command
# coverage docs2: https://llvm.org/docs/CommandGuide/llvm-profdata.html#id1
CFLAGS = -g \
	 -Wall \
	 -Wextra \
	 -Werror \
	 -Wpedantic \
	 -fprofile-instr-generate=$(COVER_DIR)/$(RAW_PROFILE_FILE) \
	 -fcoverage-mapping \
	 -I./include

# help rules
.PHONY: all
all: help

.PHONY: help
help: ## Prints help for targets with comments
	@echo "Available Rules:"
	@cat $(MAKEFILE_LIST) | grep -E '^[a-zA-Z_-]+:.*?## .*$$' | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'


## Create directories
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(COVER_DIR):
	@mkdir -p $(COVER_DIR)

$(TEST_DIR):
	@mkdir -p $(TEST_DIR)

## formula to compile .c into .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

## formula to link .o files into the main binary
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

## formula to link the .o files and the tests into the test binary
$(TEST_BIN): $(TEST_OBJS) $(TEST_SRCS) | $(TEST_DIR)
	$(CC) -g $(TEST_OBJS) $(TEST_SRCS) -lcriterion -Iinclude -o $(TEST_BIN)

# named rules start
# building and cleaning named rules
.PHONY: build
build: $(NAME) $(COVER_DIR) ## Builds the binary

.PHONY: test-build
test-build: $(TEST_BIN) ## Builds the test binary


.PHONY: clean
clean: ## Cleans transitive dependencies
	@rm -f $(OBJS)
	@rm -f $(TEST_BIN)

.PHONY: fclean 
fclean: clean ## Cleans transitive dependencies and the binary
	@rm -f $(NAME)

.PHONY: re
re: fclean build ## Rebuilds the binary

# utility rules
.PHONY: docs
docs: ## Generates documentation using Doxygen
	doxygen Doxyfile

.PHONY: test
test: test-build ## Compiles and runs tests
	@./$(TEST_BIN)

.PHONY: cov-show
cov-show: $(NAME) merge-prof ## Show code coverage in regions (the binary must've been executed at least once)
	llvm-cov show -instr-profile=$(COVER_DIR)/$(PROFILE_FILE) ./$(NAME)

.PHONY: cov-report
cov-report: $(NAME) merge-prof ## Show code coverage report (the binary must've been executed at least once)
	llvm-cov report -instr-profile=$(COVER_DIR)/$(PROFILE_FILE) ./$(NAME)

.PHONY: merge-prof
merge-prof:
	llvm-profdata merge $(COVER_DIR)/$(RAW_PROFILE_FILE) -o ./$(COVER_DIR)/$(PROFILE_FILE)
