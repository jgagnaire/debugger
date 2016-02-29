##
## Makefile for LAGEULDEBOUA
##

CC				=		g++
RM				=		rm -f
MKDIR			=		mkdir -p

CFLAGS			:= 		-Wall -Wextra -std=c++14 -Iinc/

NAME			=		geuldeboua

OBJ_DIR			=		obj
SRC_DIR			=		src

SRCS			=		${wildcard ${SRC_DIR}/*.cpp}
OBJS			=		${SRCS:${SRC_DIR}/%.cpp=${OBJ_DIR}/%.o}

all				:		${NAME}

${NAME}			:		${OBJS}
						${CC} ${CFLAGS} -o ${NAME} ${OBJS}
						@echo 'Yellow builded'

clean			:
						${RM} ${OBJS}

fclean			:		clean
						${RM} ${NAME}

re				:		fclean all

${OBJ_DIR}/%.o 	:		${SRC_DIR}/%.cpp
						@${MKDIR} ${OBJ_DIR}
						${CC} ${CFLAGS} -c $< -o $@

.PHONY			:		all clean fclean re
