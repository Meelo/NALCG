#!/bin/bash

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
    make_cmd="$make_quiet"
elif [[ "$1" == "--extra-quiet" ]]; then
    make_cmd="$make_extra_quiet"
fi

while true
do
    if [[ -z "$tracked_files" ]]; then
        declare -A tracked_files
    fi
    
    modified=false

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
        # go through unit_test folders and re-compile and re-run all tests.
        curr_location=`pwd`
        cd "$unit_test_folder"
        echo "---"
        for curr_file in *
        do
            echo "$curr_file <-"
            ls
            if [[ -d "$curr_file" ]]; then
                echo "abc"
                cd "$curr_file"
                echo "abcd"
                #make clean
                #qmake -project "CONFIG += qtestlib" -o "$curr_file.pro"
                #qmake 
                #make -j5 && "./$curr_file"
                #make clean
                cd -
            fi
        done
        cd "$curr_location"
    fi

    sleep 1s
done
# qmake -project "CONFIG += qtestlib" -o TestBoard.pro && qmake && make -j5

