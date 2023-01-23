# Compiler
COMPILER 	= g++
#VERSION 	= -std=c++17
OPTIMIZE 	= -O3
FLAGS 		= -Wall -Wextra -Werror $(VERSION) $(OPTIMIZE)
LIBS		= -L./SFML/lib -lsfml-graphics -lsfml-system -lsfml-window -lsfml-audio

# Project
NAME 		= gomoku
INCLUDE_DIR = include
INCLUDE_SFML_DIR = SFML/include
SRC_DIR		= src/
OBJECT_DIR	= obj/
INCLUDE		= game.hpp \
				visual.hpp \
				env.hpp \
				button.hpp \

SRCS 		= gomoku.cpp \
				game.cpp \
				visual.hpp \
SRC			=	$(addprefix $(SRCS_DIR),$(SRCS))
INC			=	$(addprefix $(INCLUDE_DIR)/,$(INCLUDE))
OBJ			=	$(addprefix $(OBJECT_DIR),$(SRC:%.cpp=%.o))

all: $(NAME)

$(NAME): $(OBJECT_DIR) $(OBJ) $(INC)
	$(COMPILER) $(FLAGS) -I./$(INCLUDE_DIR) -o $(NAME) $(OBJ) $(LIBS)

$(OBJECT_DIR):
	mkdir obj

$(OBJECT_DIR)%.o: $(SRC_DIR)%.cpp
	$(COMPILER) $(FLAGS) -I $(INCLUDE_DIR) -c -o $@ @<

clean:
	rm -rf obj

fclean: clean
	rm -rf $(NAME)

re: fclean all