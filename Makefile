NAME = my_traceroute

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

CC = clang
# coverage docs: https://llvm.org/docs/CommandGuide/llvm-cov.html#show-command
CFLAGS = -g -Wall -Wextra -Werror -Wpedantic -fprofile-instr-generate -fcoverage-mapping -I./include

.PHONY: all
all: help

.PHONY: help
help: ## Prints help for targets with comments
	@echo "Available Rules:"
	@cat $(MAKEFILE_LIST) | grep -E '^[a-zA-Z_-]+:.*?## .*$$' | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)


.PHONY: build
build: $(NAME) ## Builds the binary

.PHONY: clean
clean: ## Cleans transitive dependencies
	@rm -f $(OBJS)

.PHONY: fclean 
fclean: clean ## Cleans transitive dependencies and the binary
	@rm -f $(NAME)

.PHONY: re
re: fclean build ## Rebuilds the binary

.PHONY: t
t: $(NAME) ## Runs the binary for test
	@./$(NAME)

.PHONY: docs
docs: ## Generates documentation using Doxygen
	doxygen Doxyfile

.PHONY: cov-show
cov-show: $(NAME) merge-prof ## Show code coverage in regions (the binary must've been executed at least once)
	llvm-cov show -instr-profile=profile_data ./$(NAME)

.PHONY: cov-report
cov-report: $(NAME) merge-prof ## Show code coverage report (the binary must've been executed at least once)
	llvm-cov report -instr-profile=profile_data ./$(NAME)


.PHONY: merge-prof
merge-prof: 
	llvm-profdata merge default.profraw -o profile_data
