compiler = gcc-9

include_dir = include
src_dir = src
build_dir = build

source_flags = -Wall -Werror -Wpedantic
includes = -Iinclude

all:
	mkdir -p $(build_dir)
	$(compiler) $(includes) $(src_dir)/main.c $(src_dir)/lexer/lexer.c -o $(build_dir)/main

.PHONY: all
