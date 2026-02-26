#Call "make", then "./loadbalancer" (in Linux) to run. Finally, "make clean" to remove .o and executables

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRCS = src/main.cpp src/WebServer.cpp src/LoadBalancer.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = loadbalancer

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean