NAME		= gomoku
RM			= rm -rf
MKDIR		= mkdir -p

CXXFLAGS	+= -I./SFML/include -I./$(INC_PATH)
CXXFLAGS	+= -Wall -Wextra -W -std=c++11

LDFLAGS		+= -L./SFML/lib

LDFLAGS_GRAPHIC    = -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio

SRC_PATH    = src
SRC                =   gomoku.cpp \
                    game.cpp \
                    visual.cpp \
					algo.cpp \

OBJ_PATH    = obj
OBJ        = $(SRC:%.cpp=$(OBJ_PATH)/%.o)

INC_PATH    = include
INC         =   env.hpp \
                button.hpp \
                game.hpp \
                visual.hpp \
				algo.hpp \

DEP        = $(patsubst %,$(INC_PATH)/%,$(INC))

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp $(DEP)
	$(MKDIR) $(OBJ_PATH)
	g++ $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	g++ $(OBJ) $(LDFLAGS) $(LDFLAGS_GRAPHIC) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all