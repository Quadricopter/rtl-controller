SRCS	= main.c rtl_controller.c
NAME	= rtl_ctl
OBJS	= $(SRCS:.c=.o)
CFLAGS	= -Wall

all: $(OBJS)
	$(CC) -o $(NAME) $(OBJS)

clean:
	$(RM) *.o *~ \#*
	$(RM) $(NAME)
