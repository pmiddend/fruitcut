#!/bin/bash

if [[ $1 == 'run' ]]; then
	shift
	prettify_includes.py --reserved-prefix fruitcut --reserved-prefix fruitapp --reserved-prefix fruitlib --reserved-prefix sge --reserved-prefix majutsu --reserved-prefix awl --reserved-prefix mizuiro --reserved-prefix fcppt  "$@" 2>>/tmp/prettify_errors
else
	find fruitapp fruitlib \( -name '*.hpp' -o -name '*.cpp' \) | xargs $0 run
fi
