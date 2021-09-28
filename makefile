CC = g++
CXXFLAGS = -I./includes
NAME = rayTracing
SRCS_DIR = \
			./srcs
# ./MyFirstPPmFile

SRCS = main.cpp

SRC_LIST = \
			$(addprefix $(SRCS_DIR)/, $(SRCS))

OBJS = $(SRC_LIST:%.cpp=%.o)

all : $(NAME)

$(NAME) : $(OBJS) $(SRC_LIST)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean :
	rm -rf $(OBJS)

re : fclean all

fclean : clean
	rm -rf ./$(SRCS_DIR)/$(NAME)

# PHONY : 명령어와 동일한 이름이 있는 파일, 폴더가 있을 경우에도 명령어실행 하도록
.PHONY: all clean fclean re