# directories
SRC_DIR		:= src
INC_DIR		:= inc
OBJ_DIR		:= obj
LIB_DIR		:= lib

# files
NAME		:= miniRT

## sources
SRC	:=

## objects
OBJ		:= $(SRC:.c=.o)
OBJ		:= $(addprefix $(OBJ_DIR)/, $(OBJ))

## libraries etc
MLX_DIR := lib/minilibx-linux
MLX		:= libmlx.a

## dependencies
DEPS	:= $(OBJ:%.o=%.d)

## commands
CC			:= cc
LD			:= cc
RM			:= rm -rf
AR			:= ar
RANLIB		:= ranlib
MAKE		:= make
CMAKE		:= cmake
DIR_DUP		= mkdir -p $(@D)	# $(@D) = target directory

## cflags
CFLAGS		:=
CFLAGS		+= -Wall
CFLAGS		+= -Wextra
CFLAGS		+= -Werror

## cppflags
CPPFLAGS	:=
CPPFLAGS	+= -MMD
CPPFLAGS	+= -MP
CPPFLAGS	+= $(addprefix -I, $(INC_DIR))
CPPFLAGS	+= $(addprefix -I, $(FPT_DIR))
CPPFLAGS	+= $(addprefix -I, $(MLX_DIR))

## ldflags
LDFLAGS		:=
LDFLAGS		+= $(addprefix -L, $(MLX_DIR))
LDFLAGS		+= $(addprefix -l, mlx)
LDFLAGS		+= $(addprefix -l, Xext)
LDFLAGS		+= $(addprefix -l, X11)

# testing
## directories
TEST_DIR		:= test
TEST_SRC_DIR	:= $(TEST_DIR)/src
TEST_OBJ_DIR	:= $(TEST_DIR)/obj
## Unity
UNI			:= libunity.a
UNI_DIR		:= test/Unity

## sources
TEST_SRC	:=

## objects
TEST_OBJ	:= $(TEST_SRC:.c=.out)
TEST_OBJ	:= $(addprefix $(TEST_OBJ_DIR)/, $(TEST_OBJ))

## flags
TEST_CPPFLAGS	+= $(addprefix -I, $(UNI_DIR)/src)
TEST_LDFLAGS	+= $(addprefix -L, $(UNI_DIR))
TEST_LDFLAGS	+= $(addprefix -l, unity)

ifndef DEBUG
	CFLAGS += -O3
else
	CFLAGS += -O0
endif

## conditional flagsbe
ifeq ($(DEBUG), 1)
	CFLAGS += -g3
	CPPFLAGS += -g3
endif

ifeq ($(UB), 1)
	CFLAGS += -O0
	CFLAGS += -g3
	CFLAGS += -fsanitize=undefined
	CPPFLAGS += -g3
	LDFLAGS += -fsanitize=undefined
endif

ifeq ($(ASAN), 1)
	CFLAGS += -O0
	CFLAGS += -g3
	CFLAGS += -fsanitize=address
	CPPFLAGS += -g3
	LDFLAGS += -fsanitize=address
endif

ifeq ($(LSAN), 1)
	CFLAGS += -O0
	CFLAGS += -g3
	CFLAGS += -fsanitize=leak
	CPPFLAGS += -g3
	LDFLAGS += -fsanitize=leak
endif

# rules

all: $(NAME)

$(NAME): $(OBJ) $(MLX)
	@$(LD) $(OBJ) -o $@ $(LDFLAGS)
	@echo "Linking executable '$@' with LDFLAGS '$(LDFLAGS)'"

test: $(UNI) $(MLX) $(OBJ) $(TEST_OBJ)
	@$(RM) $(TEST_OBJ_DIR)
	@$(MAKE) -C $(UNI_DIR) clean > /dev/null
	@$(MAKE) -C $(MLX_DIR) clean > /dev/null

$(MLX):
	@$(MAKE) -C $(MLX_DIR) > /dev/null

$(UNI):
	@$(CMAKE) $(UNI_DIR) > /dev/null
	@$(MAKE) -C $(UNI_DIR) > /dev/null

$(TEST_OBJ_DIR)/%.out: $(TEST_SRC_DIR)/%.c
	@$(DIR_DUP)
	@$(CC) $(OBJ) $(CFLAGS) $(TEST_CFLAGS) $(CPPFLAGS) $(TEST_CPPFLAGS) -o $@ $< $(LDFLAGS) $(TEST_LDFLAGS)
	./$@
	@$(RM) $@

$(OBJ_DIR)/%.o: %.c
	@$(DIR_DUP)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
	@echo "Compiling '$@' with cflags '$(CFLAGS)' and CPPFLAGS '$(CPPFLAGS)'" 

-include $(DEPS)

clean:
	@$(RM) $(OBJ_DIR)
	@$(MAKE) -C $(MLX_DIR) clean > /dev/null

fclean: clean
	@$(RM) $(NAME) $(DEPS)

re: fclean all

run: re
	./$(NAME)

.PHONY: clean fclean re run
