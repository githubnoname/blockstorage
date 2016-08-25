.PHONY: all build clean tests

CXX = g++ -std=c++11 -Wall -Wextra -pedantic -O2 -g
LINK = g++ -std=c++11

# bins sources
STORAGE_SRC = src/main.cpp src/cxblobfiledriver.cpp src/cxblob.cpp src/cxblockmap.cpp

# tests sources
TESTBLOB_SRC = src/test_blob.cpp src/cxblobfiledriver.cpp src/cxblob.cpp src/cxblockmap.cpp

TESTS = bin/test_blob

noop =
space = $(noop) $(noop)


all: build

clean:
	rm -f objs/* bin/*

build: bin/storage

bin/storage: ${STORAGE_SRC:src/%.cpp=objs/%.d} ${STORAGE_SRC:src/%.cpp=objs/%.o}
	${LINK} ${STORAGE_SRC:src/%.cpp=objs/%.o} -o $@

bin/test_blob: ${TESTBLOB_SRC:src/%.cpp=objs/%.d} ${TESTBLOB_SRC:src/%.cpp=objs/%.o}
	${LINK} ${TESTBLOB_SRC:src/%.cpp=objs/%.o} -o $@

check: ${TESTS}
	$(subst $(space), && ,$(foreach x,${TESTS},./$(x)))

objs/%.d: src/%.cpp
	@g++ -std=c++11 $< -MM -MT $@ > $@
	@g++ -std=c++11 $< -MM -MT objs/$*.o >> $@

objs/%.o: src/%.cpp
	${CXX} -c $< -o $@

-include $(wildcard objs/*.d)
