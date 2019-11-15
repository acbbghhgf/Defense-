pwd=${shell pwd}
out_dir=.
core_dir=$(pwd)/core
os_dir=$(pwd)/os
main_dir=$(pwd)/main
third_dir=$(pwd)/third

obj_src_dir=$(pwd)/core $(pwd)/os
main_src=$(shell find ${main_dir} -name "*.c")
mains=$(patsubst %c,%o,${main_src})
third_src=$(shell find ${third_dir} -name "*.c")
thirds=$(patsubst %c,%o,${third_src})
obj_org_src=${shell find $(obj_src_dir) -name "*.c"}
obj_src=${filter-out %wtk_select.c,$(obj_org_src)}
objs=${patsubst %.c,%.o,${obj_src}}
target=libwtk.a
OBJ=WServer
TEST=WClient

CC=gcc
CFLAGS=-Wall -Werror -g -O3
LDFLAGS=
TEST_LD=-I . -L . -lm
CFLAGS+= ${TEST_LD} -D USE_TCP_NODELAY -D USE_TCP_QUICKACK


all: $(OBJ)
	echo $(OBJ)

wwk: $(target)
	echo $(target)

test:${TEST}
	echo ${TEST}

$(target): $(objs)
	$(AR) rcs $@ $^

$(OBJ): $(objs) $(mains)
	$(CC) -o $@ $^ ${CFLAGS}

$(TEST): $(objs) $(thirds)
	$(CC) -o $@ $^ ${CFLAGS}

pre: $(core_dir) $(os_dir)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< -I .. $(LDFLAGS)

clean:
	@if [ -e ${OBJ} ]; then \
		rm ${OBJ}; \
	fi;
	@if [ -e ${TEST} ]; then \
		rm ${TEST}; \
	fi;
	@if [ -e $(target) ]; then \
		rm $(target); \
	fi;
	$(shell find . -name *.o -delete)
