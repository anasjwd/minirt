NAME=	minirt
HEADER=	minirt.h

SRC=	minirt.c window_funcs.c vector_utils.c object_container.c \
		sphere_funcs.c camera.c random.c degrees_to_radians.c
OBJS=	${SRC:.c=.o}

CC=	cc
CFLAGS=	-Wall -Wextra -Werror #-g3 -fsanitize=address
MFLAGS=	-Lmlx -lmlx -Imlx -lXext -lX11 -lm
RM=	rm -f

all: ${NAME}

%.o:%.c
	${CC} ${CFLAGS} -c $< -o $@


${NAME}: ${OBJS} ${HEADER}
	${CC} ${CFLAGS} ${OBJS} ${MFLAGS} -o ${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all
