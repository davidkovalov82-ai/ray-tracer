.PHONY: all build run test clean

all: build run test

build:
	cmake -S . -B build
	cmake --build build

run:
	./build/ray_tracer

test: build
	cmake -E chdir build ctest --output-on-failure

clean:
	rm -rf build
	rm -rf images/ppm/*.ppm images/png/*.png
