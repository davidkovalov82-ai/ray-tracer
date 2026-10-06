.PHONY: all build run test clean

all: build run test

build:
	cmake -S . -B build
	cmake --build build

run:
	./build/ray_tracer

test:
	python3 test/test.py

clean:
	rm -rf build
	rm -rf images/ppm/*.ppm images/png/*.png
