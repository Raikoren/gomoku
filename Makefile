# Compiler
COMPILER 	= g++
#VERSION 	= -std=c++17
OPTIMIZE 	= -O3
FLAGS 		= -Wall -Wextra -Werror $(VERSION) $(OPTIMIZE)
LIBS		= -l:sfml-graphics -l:sfml-system -l:sfml-window -l:sfml-audio

# Project
NAME 		= gomoku
INCLUDE_DIR = include
SRC_DIR		= src/
OBJECT_DIR	= obj/
INCLUDE		= game.hpp \
				visual.hpp\

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
	rm -rf SFML

re: fclean all

dependency:
	git clone https://github.com/SFML/SFML.git