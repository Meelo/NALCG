#!/bin/bash

keep_going=true
modified=false
lock=false
function signal_handler() {
    if "$modified" ; then
        keep_going=false
        if [[ "$lock" == false ]]; then
            exit 0
        fi
    else
        modified=true
        echo "--- SIGINT caught, press ctrl+c again to exit. ---"
    fi
}

# this application potentially loops inifinitely, ctrl+c (SIGINT) will
# allow us to exit cleanly.
trap 'signal_handler' SIGINT

# define the source folder, relative to this script.
#src_folders="../src ../src/logic" 
src_folders="../src ../unit_tests" 

# define the unit test folder, relate to this script.
unit_test_folder="../unit_tests"

# define which file types are tracked.
tracked=".*\.h .*\.cpp .*\.hpp"

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

# colours
RED=`echo -e '\033[;31m'`
GREEN=`echo -e '\033[22;32m'`
ORANGE=`echo -e '\033[22;33m'`
NORM=`echo -e '\033[22;38m'`

function usage {
    # declare associative array
    declare -A instructions

    # currently I believe, that adding new items to associative array works
    # like FILO, so lets stack instructions for now. Change when proven to
    # be random or something fancier
    instructions["--help"]="Show this help message"
    instructions["--nocolour"]="Ignore colours from output"
    instructions["--nocolor"]="Ignore colours from output"
    instructions["--extra-quiet"]="Suppresses all standard and error output from tests."
    instructions["--quiet"]="Suppresses all standard output from tests."

    for key in "${!instructions[@]}"
    do
        printf "%-20s%-59s\n" "$key" "${instructions[$key]}"
    done

    # free memory, wohoo?
    unset instructions
}

if [[ "$1" == "--help" ]]; then
    usage
    exit 0
elif [[ "$1" == "--nocolour" || "$1" == "--nocolor" ]]; then
    no_colours=true
elif [[ "$1" == "--quiet" ]]; then 
    make_cmd="$make_quiet $MAKEOPTS"
elif [[ "$1" == "--extra-quiet" ]]; then
    make_cmd="$make_extra_quiet $MAKEOPTS"
fi

function colourize() {
    if [[ -z "$no_colours" ]]; then
        sed -e "s:^\(PASS.*\)$:$GREEN\1$NORM:g" \
            -e "s:^\(FAIL.*\)$:$RED\1$NORM:g" \
            -e "s:^\(SKIP.*\)$:$ORANGE\1$NORM:g"
    else
        cat
    fi
}

function run_test_at() {
    curr_file="$1"
    if [[ -n "$curr_file" ]]; then
        # lock, so if interruption is caught, 
        # the application won't immediatelly exit
        lock=true
        cd "$curr_file"
        # all these 1> /dev/nulls are for suppressing standard output
        make clean 1> /dev/null
        qmake -project "QT -= gui" "CONFIG += console qtestlib" -o "$curr_file.pro" 2> /dev/null
        qmake 1> /dev/null
        $make_cmd 1> /dev/null && "./$curr_file" | colourize
        make clean 1> /dev/null
        cd - 1> /dev/null
        lock=false
    fi
}


while "$keep_going" == true
do
    if [[ -z "$tracked_files" ]]; then
        declare -A tracked_files
    fi
    
    for dir in $src_folders
    do
        for extension in $tracked
        do
            for curr_file in `find "$dir" -iregex "$extension"`
            do
                if [[ -f "$curr_file" ]]; then
                    # getting last modified time.
                    modtime=`stat -c %Y "$curr_file"`
                    if [[ "$modtime" -gt "${tracked_files[$curr_file]}" ]]; then
                        echo "$curr_file was modified"
                        modified=true
                    fi
                    tracked_files["$curr_file"]="$modtime"
                fi
            done
        done
    done

    if [[ "$modified" == "true" ]]; then
        echo "----------------------"
        # wait for a second for interruptions.
        sleep "1s"
        echo "re-running all tests.."

        # go through unit_test folders and re-compile and re-run all tests.
        curr_location=`pwd`
        cd "$unit_test_folder" 1> /dev/null
        for curr_file in *
        do
            if [[ -d "$curr_file" ]]; then
                run_test_at "$curr_file"
            fi
        done
        cd "$curr_location" 1> /dev/null
        modified=false
    fi

    sleep "1s"
done
