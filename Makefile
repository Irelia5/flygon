sources := $(wildcard *.cc)
objects := $(patsubst %.cc, %.o, $(sources))
deps := $(patsubst %.cc, %.d, $(sources))

INCLUDES := -I.
CC := g++
CPPFLAGS := -std=c++11

TARGET := main

all: $(TARGET) $(deps) $(objects)

$(TARGET): $(objects)
	$(CC) -o $@ $^

$(objects): %.o: %.cc
	@echo "正在编译模块"
	$(CC) -c $(CPPFLAGS) $(INCLUDES) $< -o $@

$(deps): %.d: %.cc
	@echo "正在编译模块2"
	@set -e; rm -f $@; \
    $(CC) -MM $(CPPFLAGS) $(INCLUDES) $< > $@.$$$$; \
    sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
    rm -f $@.$$$$

include $(sources:.cc=.d)

.PHONY: clean
clean:
	-rm $(objects) main $(deps)