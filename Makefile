NAME=NLSP

SRCS = main.cpp ./logger/logger.cpp ./rpc/rpc.cpp
OBJS = $(SRCS:.cpp=.o)
CXXFLAGS = -Wall -Wextra -Werror -std=c++17

%.o: %.cpp
	g++ $(CXXFLAGS) -c -o $@ $<

$(NAME): $(OBJS)
	g++ $(CXXFLAGS) -o $(NAME) $(OBJS)

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -rf $(NAME)
	rm log.txt

.PHONY: clean fclean
