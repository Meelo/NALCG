#!/bin/bash

keep_going=true
modified=false
function signal_handler() {
    if "$modified" ; then
        keep_going=false
    else
        modified=true
    fi
}
trap 'signal_handler' SIGINT

# define the source folder, relative to this script.
src_folders="../src ../src/logic ../unit_tests" 

# define the unit test folder, relate to this script.
unit_test_folder="../unit_tests/"

# define which file types are tracked.
tracked="*.h *.cpp *.hpp"

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
    instructions["--help"]="Show this help message"
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
elif [[ "$1" == "--quiet" ]]; then 
    make_cmd="$make_quiet $MAKEOPTS"
elif [[ "$1" == "--extra-quiet" ]]; then
    make_cmd="$make_extra_quiet $MAKEOPTS"
fi

function run_test_at() {
    curr_file="$1"
    if [[ -n "$curr_file" ]]; then
        cd "$curr_file"
        make clean 1> /dev/null
        qmake -project "CONFIG += qtestlib" -o "$curr_file.pro" 2> /dev/null
        qmake 1> /dev/null
        $make_cmd 1> /dev/null && "./$curr_file"
        make clean 1> /dev/null
        cd - 1> /dev/null
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
            for curr_file in $dir/$extension
            do
                if [[ -f "$curr_file" ]]; then
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
# qmake -project "CONFIG += qtestlib" -o TestBoard.pro && qmake && make -j5

