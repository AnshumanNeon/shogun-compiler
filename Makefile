compiler = gcc-9

include_dir = include
src_dir = src
build_dir = build

C_flags = -Wall -Werror -Wpedantic -g
includes = -Iinclude

all:
	mkdir -p $(build_dir)
	$(compiler) $(C_flags) $(includes) $(src_dir)/main.c $(src_dir)/lexer/lexer.c -o $(build_dir)/main

.PHONY: all
