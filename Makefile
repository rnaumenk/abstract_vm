# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rnaumenk <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/07 18:45:57 by rnaumenk          #+#    #+#              #
#    Updated: 2018/05/07 18:46:00 by rnaumenk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	avm
LIB			=	libavm.a
INC_DIR		=	./include/
INC_FILES	=	./avm.hpp \
				./Factory.hpp \
				./IOperand.hpp \
				./Operand.hpp \
				./AvmException.hpp \
				./Lexer.hpp \
				./Parser.hpp \
				./MyStack.hpp
SRC_DIR		=	./src/
SRC			= 	./main.cpp \
				./Factory.cpp \
				./AvmException.cpp \
				./Lexer.cpp \
				./Parser.cpp \
				./global.cpp
OBJ_DIR		=	./obj/
OBJ			=	$(addprefix $(OBJ_DIR), $(SRC:.cpp=.o))
INC			=	$(addprefix $(INC_DIR), $(INC_FILES))

all: $(NAME)

$(NAME): $(LIB)
	@clang++ -std=c++11 -Wall -Wextra -Werror -o $@ $^
	@echo "\033[4;32m>> $(NAME) is ready <<\033[0m"

$(LIB): $(OBJ)
	@ar rc $@ $^
	@ranlib $@
	@echo "\033[3;36m>> $(LIB) has been made <<\033[0m"

$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp $(INC)
	@mkdir -p $(OBJ_DIR)
	@clang++ -std=c++11 -Wall -Wextra -Werror -o $@ -c $<

clean:
	@rm -rf $(LIB) $(OBJ_DIR)
	@echo "\033[3;33m>> $(LIB) and $(OBJ_DIR) have been deleted <<\033[0m"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[3;31m>> $(NAME) has been removed <<\033[0m"

re: fclean all

.PHONY: all clean fclean re
