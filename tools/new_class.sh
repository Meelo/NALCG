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
source_folder="../src"
header_template="new_class.h.template"
source_template="new_class.cpp.template"

# define this according to the project name
project_name="NALCG"

# defining keywords used in templates
key_project_guard="##GPROJECT#"
key_class_guard="##GCLASS_NAME#"
key_class_filename="##CLASS_FILENAME#"
key_class_name="##CLASS_NAME#"

function usage {
    echo "usage sh: $0 class1 class2 class3.."
    echo "Please use CamelCasing in class names."
}

if [[ "$#" -ge "$NEW_CLASSES" ]]; then
    for class in "$@"
    do
        project_guard=`echo "$project_name" | $TO_UPPER`
        class_guard=`echo "$class" | sed -r 's:([A-Z]):_\1:g' | $TO_UPPER`
       
        # all file names are lower case, according to coding convention.
        filename=`echo "$class" | $TO_LOWER`
        header_file="$source_folder/$filename.h"
        source_file="$source_folder/$filename.cpp"
        
        # only create header file if such doesn't exist
        if [[ ! -f "$header_file" ]]; then
            cp "$header_template" "$header_file"

            # add project name to guard
            sed -e "s:$key_project_guard:$project_guard:g" -i "$header_file"

            # add class name to guard
            sed -e "s:$key_class_guard:$class_guard:g" -i "$header_file"

            # add class name to where it is needed
            sed -e "s:$key_class_name:$class:g" -i "$header_file"

            echo "$header_file generated"
        fi

        # only create source file if such doesn't exist
        if [[ ! -f "$source_file" ]]; then
            cp "$source_template" "$source_file"
            
            # class's filename is used in includes
            sed -e "s:$key_class_filename:$filename:g" -i "$source_file"
            
            # Class methods requires the name of the class
            sed -e "s:$key_class_name:$class:g" -i "$source_file"

            echo "$source_file generated"
        fi

        
    done
else
    usage
fi

# cleaning
cd "$where_was_I"
