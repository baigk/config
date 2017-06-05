
CXXFLAGS +=  -std=c++11 -g -ggdb -fpermissive

INCLUDES = -I../lib

LIBS_PATH = -L../lib
LIBS = ../lib/librdkafka++.a ../lib/librdkafka.a -lpthread -lrt -ldl

objs = producer.o main.o consumer.o

.PHONY: all
all: $(objs)
	g++ $(CXXFLAGS) $(INCLUDES) $^ -o $@  $(LIBS)

%.d : %.cpp
	@rm -rf $@; \
	set -e; rm -f $@; \
	$(CC) -MM $(CXXFLAGS) $(INCLUDES) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o : %.cpp
	g++ $(CXXFLAGS) $(INCLUDES) -c $< -o $@

include $(objs:.o=.d)

clean:
	rm -f $(objs)

