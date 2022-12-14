#!/bin/bash

print_help () {
    echo "
Script to setup a dev environment where to test the Azure IoT edge modules under development.

Usage:
    ./run_dev_module -c <connection string> -d <destination directory>

Arguments:
    -c|--conn-string : azure device connection string containing hostname, device ID and share access key
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
    -c|--conn-string)
      CONNECTION_STRING="$2"
      shift # past argument
      shift # past value
      ;;
    -d|--directory)
      DEST_DIR="$2"
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

check_vars "${CONNECTION_STRING}" "connection string"
check_vars "${DEST_DIR}" "destination directory"

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
WORK_DIR="${DIR}/../dev/${DEST_DIR}"
echo ${WORK_DIR}

python3 -m virtualenv .env --python=3.9

# Enter the environment
source .env/bin/activate

python3 -m pip install -r requirements.txt

cd ${WORK_DIR}

# The setup step requires the sudo permissions
sudo iotedgehubdev setup -c ${CONNECTION_STRING} -g 'edgehub'
iotedgehubdev start -d config/deployment.amd64.json -v