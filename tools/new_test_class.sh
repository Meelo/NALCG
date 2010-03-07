#!/bin/bash

# initializing
where_was_I=`pwd`
script_location=`readlink -f "$0"`
where_should_I_be=`dirname "$script_location"`
cd "$where_should_I_be"

# inner constants
NEW_CLASSES=1
TO_UPPER='tr "[:lower:]" "[:upper:]"'
TO_LOWER='tr "[:upper:]" "[:lower:]"'

# change this if src-folder is somewhere else
source_folder="../unit_tests"
test_template="new_test_class.template"

# defining keywords used in templates
key_class_name="##TEST_CLASS_NAME#"
key_class_filename="##TEST_CLASS_FILENAME#"

function usage {
    echo "usage sh: $0 TestClass1 TestClass2 TestClass3.."
    echo "Please use CamelCasing in class names."
}

if [[ "$#" -ge "$NEW_CLASSES" ]]; then
    for class in "$@"
    do
        # all file names are lower case, according to coding convention.
        filename=`echo "$class" | $TO_LOWER`
        test_file="$source_folder/$filename.cpp"
        
        # only create header file if such doesn't exist
        if [[ ! -f "$test_file" ]]; then
            cp "$test_template" "$test_file"

            # add class name to where it is needed
            sed -e "s:$key_class_name:$class:g" -i "$test_file"

            # add class's filename to its include section..
            sed -e "s:$key_class_filename:$filename:g" -i "$test_file"

            echo "$test_file generated"
        fi
    done
else
    usage
fi

# cleaning
cd "$where_was_I"
