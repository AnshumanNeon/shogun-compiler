compiler = gcc-9

include_dir = include
src_dir = src
build_dir = build

C_flags = -Wall -Werror -Wpedantic
includes = -Iinclude

provide_build_dir:
	mkdir -p $(build_dir)

all: provide_build_dir
	$(compiler) $(C_flags) $(includes) $(src_dir)/main.c $(src_dir)/lexer/lexer.c $(src_dir)/parser/parse.c -o $(build_dir)/main

debug: provide_build_dir
	$(compiler) $(C_flags) -g $(includes) $(src_dir)/main.c $(src_dir)/lexer/lexer.c -o $(build_dir)/main

run: provide_build_dir
	$(compiler) $(C_flags) $(includes) $(src_dir)/main.c $(src_dir)/lexer/lexer.c -o $(build_dir)/main
	$(build_dir)/main test/test1.sg

.PHONY: all run debug
