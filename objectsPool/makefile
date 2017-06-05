
FLAGS = -std=c++11 -g -ggdb -fpermissive

src  = $(wildcard *.cpp)
objs = $(src:.cpp=.o)

all: $(objs)
	g++ $(FLAGS) -o $@ $^ -lpthread

include $(src:.cpp=.d)

%.d: %.cpp
	set -e; rm -f $@; \
	g++ -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o: %.cpp
	g++ $(FLAGS) -c $< -o $@ 

