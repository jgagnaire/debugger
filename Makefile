##
## Makefile for LAGEULDEBOUA
##

CC				=		g++
RM				=		rm -f
MKDIR			=		mkdir -p

CFLAGS			:= 		-Wall -Wextra -Iinc/ -std=c++11
CDEBUGFLAGS		:=		-g -fvar-tracking
LDFLAGS			:=		-ldwarf -lelf

NAME			=		gueuldeboua
DEBUG_NAME		=		debug_program/debug

OBJ_DIR			=		obj
SRC_DIR			=		src

SRCS			=		${wildcard ${SRC_DIR}/*.cpp}
OBJS			=		${SRCS:${SRC_DIR}/%.cpp=${OBJ_DIR}/%.o}

all				:		${NAME} ${DEBUG_NAME}

${NAME}			:		${OBJS}
						${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${LDFLAGS}
						@echo ${NAME} ' build'

${DEBUG_NAME}	:
						gcc ${CDEBUGFLAGS} -o ${DEBUG_NAME} debug_program/main.c
						@echo ${DEBUG_NAME} ' build'

clean			:
						${RM} ${OBJS}

fclean			:		clean
						${RM} ${NAME}
						${RM} ${DEBUG_NAME}

re				:		fclean all

${OBJ_DIR}/%.o 	:		${SRC_DIR}/%.cpp
						@${MKDIR} ${OBJ_DIR}
						${CC} ${CFLAGS} -c $< -o $@

.PHONY			:		all clean fclean re
