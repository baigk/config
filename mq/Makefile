
CXXFLAGS +=  -std=c++11 -g -ggdb

INCLUDES = -I./lib \
		   -I./ \
		   -I./kafka

LIBS_PATH = -L./lib
LIBS = ./lib/librdkafka++.a ../lib/librdkafka.a -lpthread -lrt -ldl

VPATH = ./
VPATH += ./kafka

SRCS = $(wildcard *.cpp)  
SRCS += $(wildcard kafka/*.cpp)  

OBJS = $(patsubst %.cpp,%.o,$(SRCS))

.PHONY: all
all: $(OBJS)
	g++ $(CXXFLAGS) $(INCLUDES) $^ -o $@  ./temp/$(LIBS)

%.d : %.cpp
	@rm -rf ./temp/$@; \
	set -e; rm -f ./temp/$@; \
	$(CC) -MM $(CXXFLAGS) $(INCLUDES) $< > ./temp/$@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > ./temp/$@; \
	rm -f ./temp/$@.$$$$

%.o : %.cpp
	g++ $(CXXFLAGS) $(INCLUDES) -c $< -o ./temp/$@

include $(./temp/OBJS:.o=.d)

clean:
	rm -f ./temp/$(OBJS)

