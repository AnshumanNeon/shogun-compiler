compiler = gcc-9

source_flags = -Wall -Werror -Wpedantic

src_dir = src
build_dir = build

all:
	mkdir -p $(build_dir)
	$(compiler) $(src_dir)/main.c -o $(build_dir)/main

.PHONY: all
