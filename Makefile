##
## Makefile for LAGEULDEBOUA
##

CC				=		g++
RM				=		rm -f
MKDIR			=		mkdir -p

CFLAGS			:= 		-Wall -Wextra -Iinc/ -g
LDFLAGS			:=		-ldwarf -lelf

NAME			=		gueuldeboua

OBJ_DIR			=		obj
SRC_DIR			=		src

SRCS			=		${wildcard ${SRC_DIR}/*.cpp}
OBJS			=		${SRCS:${SRC_DIR}/%.cpp=${OBJ_DIR}/%.o}

all				:		${NAME}

${NAME}			:		${OBJS}
						${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${LDFLAGS}
						@echo 'LAGUEULDEBOUA build'

clean			:
						${RM} ${OBJS}

fclean			:		clean
						${RM} ${NAME}

re				:		fclean all

${OBJ_DIR}/%.o 	:		${SRC_DIR}/%.cpp
						@${MKDIR} ${OBJ_DIR}
						${CC} ${CFLAGS} -c $< -o $@

.PHONY			:		all clean fclean re
