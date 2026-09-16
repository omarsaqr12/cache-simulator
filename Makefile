CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2

.PHONY: all clean test gui install-deps
all: cache_simulator

cache_simulator: src/main.cpp include/cache.h
	$(CXX) $(CXXFLAGS) -o $@ src/main.cpp

# Compatibility executable, using the same validated parser and cache model.
cache_logic: src/cache_logic.cpp src/main.cpp include/cache.h
	$(CXX) $(CXXFLAGS) -o $@ src/cache_logic.cpp

build/test_cache: tests/test_cache.cpp include/cache.h
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ tests/test_cache.cpp

test: cache_simulator build/test_cache
	./build/test_cache
	python3 -m unittest discover -s tests -p 'test_*.py' -v

install-deps:
	python3 -m pip install -r requirements.txt

gui: cache_simulator
	python3 src/gui.py

clean:
	rm -f cache_simulator cache_logic cache_simulator.exe cache_logic.exe
	rm -rf build
