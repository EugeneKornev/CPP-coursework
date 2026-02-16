cxx = g++
ld = g++
cxx_flags = -std=c++23 -O2 -fsanitize=address,undefined

exec = task_1/task_1

all: $(exec)

#clean:
#	rm -rf $(exec)

task1: task_1/avl.cpp task_1/test.cpp task_1/avl.hpp
	$(cxx) $^ $(cxx_flags) -o $@