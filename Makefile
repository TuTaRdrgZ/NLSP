NAME=NLSP

SRCS = main.cpp ./logger/logger.cpp ./rpc/rpc.cpp
INCLUDES = -I./logger -I./rpc -I./norminette/ -I ./lsp/
OBJS = $(SRCS:.cpp=.o)
CXXFLAGS = -Wall -Wextra -Werror

%.o: %.cpp
	g++ $(CXXFLAGS) -c -o $@ $< $(INCLUDES)

$(NAME): $(OBJS)
	g++ $(CXXFLAGS) -o $(NAME) $(OBJS) $(INCLUDES)

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -rf $(NAME)

.PHONY: clean fclean
