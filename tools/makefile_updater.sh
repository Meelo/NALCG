#!/bin/bash

# initializing
relative_src_location="../src"
where_was_I=`pwd`
src_dir=`readlink -f "$relative_src_location"`
if [[ "$where_was_I" != "$src_dir" ]]; then
    echo "Switching from $where_was_I to $src_dir"
    cd "$src_dir"
fi

# for linker options
GCCFLAGS=`pkg-config --libs --cflags CEGUI-OGRE OIS OGRE CEGUI`

function usage {
    echo "usage: sh $0 class1 class2 class3"
    echo "to update all dependencies: sh $0 --all"
}
function fixLinkerFlags() {
    sed -e "s:^\(LINKER_FLAGS=\).*$:\1$GCCFLAGS:g" -i Makefile
}

if [[ $# == 0 ]]; then
    usage
    echo -e "\nValid files:"
    #files="`find . -iregex '.*\.cpp'`"
    files="*.cpp logic/*.cpp view/*.cpp"
    for f in $files
    do
        echo -en "${f%.cpp} "
    done
    echo ""
else
    # check if user wants to go through all dependencies
    if [[ $1 == "--all" ]]; then
        #files="`find . -iregex '.*\.cpp'`"
        files="*.cpp logic/*.cpp view/*.cpp"
    else
        files=$@
    fi
    fixLinkerFlags
    for f in $files
    do
        # parse off the extension
        file=${f%.cpp}
        filepath="${f%/*}/"

        if [[ "${f}/" == "${filepath}" ]]; then
            filepath=""
        fi
        if [[ -f "$file.cpp" ]]; then
            # check if target is already in the OBJS-list
            objs=`grep "^OBJS=.*$file.o" Makefile`

            # add target to OBJS-list if it is missing
            if [[ -z "$objs" ]]; then
                sed -e "s%\(^OBJS=.*\)%\1 $file.o%g" -i Makefile
                echo "object $file.o added to list of known objects"
            fi
        else
            echo "file not found: $f / $file"
            usage
        fi
    done
fi

if [[ "$where_was_I" != "`pwd`" ]]; then
    echo "Switching back to $where_was_I"
    cd "$where_was_I"
fi
