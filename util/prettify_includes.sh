#!/bin/bash

if [[ $1 == 'run' ]]; then
	shift
	prettify_includes.py --reserved-prefix fruitcut --reserved-prefix fruitapp --reserved-prefix fruitlib --reserved-prefix sge --reserved-prefix majutsu --reserved-prefix awl --reserved-prefix mizuiro --reserved-prefix fcppt  "$@" 2>>/tmp/prettify_errors
else
	find src/fruitapp src/fruitlib src/sandbox src/test include/fruitapp include/fruitlib \( \( -name '*.hpp' -o -name '*.cpp' \) -a ! -path 'src/model/obj/basic_tokens_decl.hpp' -a ! -path 'src/model/obj/basic_grammar_decl.hpp' -a ! -path 'src/model/obj/token.hpp' -a ! -path 'src/plugins/opengl/common.hpp' \) | xargs $0 run
fi
