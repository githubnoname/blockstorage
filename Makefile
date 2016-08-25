.PHONY: all build clean tests

CXX = g++ -std=c++11 -Wall -Wextra -pedantic -O2 -g
LINK = g++ -std=c++11

# bins sources
STORAGE_SRC = src/main.cpp src/cxblobfiledriver.cpp src/cxblob.cpp src/cxblockmap.cpp

# tests sources
# READERSTEST_SRC = src/test_readers.cpp src/cxfilereader.cpp
# PARSERTEST_SRC  = src/test_parser.cpp src/cxparser.cpp
# FILTERSTEST_SRC = src/test_filters.cpp src/axcmp.cpp src/cxpipefilter.cpp
# SORTTEST_SRC    = src/test_sort.cpp src/axcmp.cpp src/cxpipefilter.cpp src/cxpipesorter.cpp

# TESTS = bin/test_readers bin/test_parser bin/test_filters bin/test_sort

noop =
space = $(noop) $(noop)


all: build

clean:
	rm -f objs/* bin/*

build: bin/storage

bin/storage: ${STORAGE_SRC:src/%.cpp=objs/%.d} ${STORAGE_SRC:src/%.cpp=objs/%.o}
	${LINK} ${STORAGE_SRC:src/%.cpp=objs/%.o} -o $@

check: ${TESTS}
	$(subst $(space), && ,$(foreach x,${TESTS},./$(x)))

objs/%.d: src/%.cpp
	@g++ -std=c++11 $< -MM -MT $@ > $@
	@g++ -std=c++11 $< -MM -MT objs/$*.o >> $@

objs/%.o: src/%.cpp
	${CXX} -c $< -o $@

-include $(wildcard objs/*.d)
