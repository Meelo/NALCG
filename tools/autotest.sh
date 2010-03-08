#!/bin/bash

# define the source folder, relative to this script.
src_folder="../src"

# define the unit test folder, relate to this script.
unit_test_folder="../unit_tests/"

# define how the tests should be compiled.
# qmake project initialization
qMake_init='qmake -project "CONFIG += qtestlib" '
# qmake Makefile generating
qMake_gen='qmake'
# make and its params.
MAKEOPTS="-j5"
make_cmd="make $MAKEOPTS"
make_quiet="make $MAKEOPTS 1>/dev/null"
make_extra_quiet="make $MAKEOPTS 2>/dev/null 1>/dev/null"

function usage {
    # declare associative array
    declare -A instructions

    # currently I believe, that adding new items to associative array works
    # like FILO, so lets stack instructions for now. Change when proven to
    # be random or something fancier
    instructions["--extra-quiet"]="Suppresses all standard and error output from tests."
    instructions["--quiet"]="Suppresses all standard output from tests."

    for key in "${!instructions[@]}"
    do
        printf "%-20s%-59s\n" "$key" "${instructions[$key]}"
    done

    # free memory, wohoo?
    unset instructions
}

if [[ "$1" == "--quiet" ]]; then 
    make_cmd="$make_quiet"
elif [[ "$1" == "--extra-quiet" ]]; then
    make_cmd="$make_extra_quiet"
fi

usage
