# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ayarmaya <ayarmaya@student.42nice.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/30 22:36:01 by ayarmaya          #+#    #+#              #
#    Updated: 2025/05/30 22:39:11 by ayarmaya         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= BitcoinExchange
CC				:= c++
CFLAGS			:= -Wall -Wextra -Werror -std=c++98 -MMD -MP
LDFLAGS			:= -Wall -Wextra -Werror -std=c++98 -MMD -MP
RM				:= rm -f
RMDIR 			:= rm -rf
INCDIR 			:= include
HEADERS 		:= -I./$(INCDIR)
SRCDIR			:= srcs
SRCS			:= $(SRCDIR)/main.cpp \
				   $(SRCDIR)/BitcoinExchange.cpp

OBJDIR 			:= obj
OBJS 			:= $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))
DEPS 			:= $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.d, $(SRCS))

all 			: $(NAME)

$(NAME) 		: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o 	: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) $(HEADERS) -c $< -o $@

clean:
	$(RMDIR) $(OBJDIR)

fclean 			: clean
	$(RM) $(NAME)

re 				: fclean all

-include $(DEPS)

.PHONY 			: all clean fclean re