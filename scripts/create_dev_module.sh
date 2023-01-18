#!/bin/bash

print_help () {
    echo "
Script to setup a dev environment where to test the Azure IoT edge modules under development.

Usage:
    ./create_dev_module -t <template> -m <module name> -d <destination directory>

Arguments:
    -t|--template    : template type in which the module will be implemented (c|csharp|java|nodejs|python|csharpfunction)
    -m|--module      : module name
    -d|--directory   : destination directory where the developement environment will be generated
    -h|--help        : prints this help message
"
}

# Check whether the arguments are correctly set
check_vars () {
    var_name=$1
    var_type=$2

    if [ "${var_name}" == "" ]; then
        echo "Error: ${var_type} string not provided"

        print_help
        exit 1
    fi
}

while [[ $# -gt 0 ]]; do
  case $1 in
    -d|--directory)
      DEST_DIR="$2"
      shift # past argument
      shift # past value
      ;;
    -t|--template)
      TEMPLATE="$2"
      shift # past argument
      shift # past value
      ;;
    -m|--module)
      MODULE="$2"
      shift # past argument
      shift # past value
      ;;
    -h|--help)
      print_help
      exit 1
      ;;
    -*|--*)
      echo "Unknown option $1"
      print_help
      exit 1
      ;;
  esac
done

check_vars "${DEST_DIR}" "destination directory"
check_vars "${TEMPLATE}" "template type"
check_vars "${MODULE}" "module name"

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
WORK_DIR="${DIR}/../dev/${DEST_DIR}"
echo ${WORK_DIR}

python3 -m virtualenv .env --python=3.9

# Enter the environment
source .env/bin/activate

python3 -m pip install -r requirements.txt

rm -r ${WORK_DIR}
mkdir ${WORK_DIR}
cd ${WORK_DIR}

iotedgedev new -m ${MODULE} -t ${TEMPLATE} .