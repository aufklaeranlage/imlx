# directories
SRC_DIR		:= src
INC_DIR		:= inc
OBJ_DIR		:= obj
LIB_DIR		:= lib

# files
NAME		:= libimlx.a

## sources
SRC	:=
vpath %.c $(SRC_DIR)
SRC += session.c
SRC += window.c
SRC += image.c
SRC += draw.c
SRC += ppmconv.c

## objects
OBJ		:= $(SRC:.c=.o)
OBJ		:= $(addprefix $(OBJ_DIR)/, $(OBJ))

## libraries etc
ifndef MLX_DIR
	MLX_DIR := lib/mlx
endif
ifndef MLX
	MLX		:= libmlx.a
endif

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

## ldflags.gen
LDFLAGS		:=

# testing
## directories
TEST_DIR		:= test
TEST_SRC_DIR	:= $(TEST_DIR)/src
TEST_OBJ_DIR	:= $(TEST_DIR)/obj

## sources
TEST_SRC	:=
vpath %.c $(TEST_SRC_DIR)
# TEST_SRC	+= multiple_windows.c
# TEST_SRC	+= ppm_conv.c

## objectTEST_SRC	+= ppm_conv.c
TEST_OBJ	:= $(TEST_SRC:.c=.out)
TEST_OBJ	:= $(addprefix $(TEST_OBJ_DIR)/, $(TEST_OBJ))

## flags
TEST_CPPFLAGS	+= $(addprefix -I, $(UNI_DIR)/src)

TEST_LDFLAGS	+= $(addprefix -L, $(MLX_DIR))
TEST_LDFLAGS	+= $(addprefix -l, mlx)
TEST_LDFLAGS	+= $(addprefix -l, Xext)
TEST_LDFLAGS	+= $(addprefix -l, X11)

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

$(NAME): $(OBJ)
	@echo "Creating library '$@' from files '$(OBJ)'"
	@$(AR) rcs $@ $(OBJ)

test: fclean $(MLX) $(OBJ) $(TEST_OBJ)
	@$(MAKE) -C $(MLX_DIR) clean > /dev/null
	@$(RM) $(TEST_OBJ_DIR)

$(MLX):
	@$(MAKE) -C $(MLX_DIR) > /dev/null

$(UNI):
	@$(CMAKE) $(UNI_DIR) > /dev/null
	@$(MAKE) -C $(UNI_DIR) > /dev/null

$(TEST_OBJ_DIR)/%.out: %.c
	@$(DIR_DUP)
	$(CC) $(CFLAGS) $(TEST_CFLAGS) $(CPPFLAGS) $(TEST_CPPFLAGS) -o $@ $< $(OBJ) $(LDFLAGS) $(TEST_LDFLAGS)
	@echo "Compiling '$@' with cflags '$(CFLAGS) $(TEST_CFLAGS)' and CPPFLAGS '$(CPPFLAGS) $(TEST_CPPFLAGS)'" 
	$(TEST_PRE) ./$@
	@$(RM) $@

$(OBJ_DIR)/%.o: %.c
	@$(DIR_DUP)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
	@echo "Compiling '$@' with cflags '$(CFLAGS)' and CPPFLAGS '$(CPPFLAGS)'" 

-include $(DEPS)

clean:
	@$(RM) $(OBJ_DIR)
	@$(MAKE) -C $(MLX_DIR) clean > /dev/null
	@echo "Deleting '$(OBJ_DIR)' and cleaning mlx"

fclean: clean
	@$(RM) $(NAME) $(DEPS)
	@echo "Deleting library '$(NAME)'"

re: fclean all

run: re
	./$(NAME)

.PHONY: clean fclean re run
