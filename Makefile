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

## ldflags.gen
LDFLAGS		:=

ifndef DEBUG
	CFLAGS += -O3
else
	CFLAGS += -O0
endif

## conditional flags
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

$(OBJ_DIR)/%.o: %.c
	@$(DIR_DUP)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
	@echo "Compiling '$@' with cflags '$(CFLAGS)' and CPPFLAGS '$(CPPFLAGS)'" 

-include $(DEPS)

clean:
	@$(RM) $(OBJ_DIR)
	@echo "Deleting '$(OBJ_DIR)' and cleaning mlx"

fclean: clean
	@$(RM) $(NAME) $(DEPS)
	@echo "Deleting library '$(NAME)'"

re: fclean all

.PHONY: clean fclean re
