# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Program name
NAME = minishell

# Directories
SRC_DIR = src
OBJ_DIR = obj
INCS = -I includes
LDLIBS = -lreadline

# Colors for output
GREEN = \033[0;92m
BLUE = \033[0;94m
CYAN = \033[0;96m
YELLOW = \033[0;93m

# Source files (all with correct relative paths and .c extensions)
SRC = read.c quotes.c libft.c ft_split.c expand.c chained_list.c ft_itoa.c \
      heredoc/get_next_line.c heredoc/heredoc.c heredoc/heredoc_utlis.c \
      execution_posix/exec.c execution_posix/exec_recursion.c \
      cmd/cmd1.c cmd/cmd2.c \
      ast/new_ast.c \

# Object files, with obj/ prefix and .o extension
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# Directories needed for object files (unique and sorted)
DIRS := $(sort $(dir $(OBJS)))

# Default target
all: $(NAME)

# Link objects into archive/library
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCS) $(LDLIBS) -o $@ $^
	@echo "$(GREEN)minishell compiled successfully!$(BLUE)"

# Create directories for object files
$(DIRS):
	@mkdir -p $@

# Compile .c files to .o files, ensure directory exists first
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(DIRS)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@
	@echo "$(CYAN)Compiled: $<$(BLUE)"

# Clean object files and directories
clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(YELLOW)minishell object files cleaned!$(BLUE)"

# Clean everything including executable
fclean: clean
	@rm -f $(NAME)
	@echo "$(CYAN)minishell executable files removed!$(BLUE)"

# Rebuild everything
re: fclean all

# Phony targets
.PHONY: all clean fclean re
