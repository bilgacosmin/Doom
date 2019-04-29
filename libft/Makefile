NAME = libft.a

SRCS = $(shell ls | grep -E ".+\.c")

OBJS = $(SRCS:.c=.o)

all: $(NAME)
 
$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)

%.o : %.c
	gcc -Wall -Wextra -Werror -o $@ -c $< -I includes/

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f libft.a

re: fclean $(NAME)