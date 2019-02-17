CXXFLAGS=-O3 -std=c++14 -DNDEBUG
CXX=clang++
all: perf sample
clean:
	rm -f perf sample
