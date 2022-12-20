# Huxon Containerized Sensor Examples

The [Huxon platform](https://huxon.huxelerate.it/docs/index.html) allows to define nodes that include the so-called **Containerized Sensors**.

A **Containerized Sensor** is an element in the infrastructure capable of generating data, similarly to a sensor of a [Sensor Board](https://huxon.huxelerate.it/docs/infrastructure_setup.html#sensor-boards-setup).

Each **containerized sensor** has a set of images that can be deployed and run in a docker container, generating the data and making it available in the user code, and to the
rest of the infrastructure nodes involved in the deployment.

The images are none other than Azure IoT Hub modules that can be deployed on an device, therefore we will refer to them as **modules** from now on.

The **containerized sensor** is useful when you want to import any existing code that produces data, inside the Huxon platform.

Please, refer to the guides provided by Azure to understand how to construct and build the module using different languages: 

- [C](https://learn.microsoft.com/en-us/azure/iot-edge/tutorial-c-module?view=iotedge-2020-11>)
- [C#](https://learn.microsoft.com/en-us/azure/iot-edge/tutorial-csharp-module?view=iotedge-2020-11)
- [Java](https://learn.microsoft.com/en-us/azure/iot-edge/tutorial-java-module?view=iotedge-2020-11)
- [Node.js](https://learn.microsoft.com/en-us/azure/iot-edge/tutorial-node-module?view=iotedge-2020-11)
- [Python](https://learn.microsoft.com/en-us/azure/iot-edge/tutorial-python-module?view=iotedge-2020-11)

This repository contains a set of examples that can be used to quickly test out-of-the box the **Containerized Sensor** feature of the Huxon platform.

## Prerequisites

In order to test and use the containerized sensors, the following prerequisites must be met:

- [Huxon account](https://huxon.huxelerate.it/) to interface with the platform
- [Docker](https://docs.docker.com/get-docker/) to build the images for the Azure modules
- python to create and run the IoT edge modules
- npm to create NodeJS-based modules

To develop the modules and test them locally please run the following from the root directory of this repository:

```bash
# Installs the python dependencies
python3 -m pip install iotedgedev

# In case a python module is used
python3 -m pip install cookiecutter

# In case a nodejs module is used
npm install -g yo # Yeoman
npm install -g generator-azure-iot-edge-module
```

## Quick build

To build the example docker images, run the following:

```bash
cd examples/<preferred language>
docker build -t <name of the sensor> -f Dockerfile --build-arg ARCH_TYPE=<architecture type> . 
```

Note that the architecture type can be provided as a build argument, and it needs to match the target architecture, where the module will run. E.g. a raspberry is based on ARM, therefore ARCH_TYPE needs to be `arm32v7`.

You may now push the docker image to your private/public registry and use it within the [Huxon platform](https://huxon.huxelerate.it/), when defining the corresponding containerized sensor.

## Develop your own module

To develop a custom module, follow the steps below, or otherwise use the dedicated scripts to automate the process.

### Automatic setup

1. Run the module creation step

```bash
bash scripts/create_dev_module.sh -m <module-name> -t <template> -d <destination-dir>
```

Where the arguments are:
- -t, --template  : template type representing the language used to implement the module (c, csharp, java, nodejs, python, csharpfunction)
- -m, --module    : module name
- -d, --directory : destination directory where the developement environment will be generated

2. Add your changes to the generated module under the chosen destination directory. You can find generated code
   in the language specified by in the `template` argument

3. Run the module simulation step

```bash
bash scripts/run_dev_module.sh -c <connection-string> -d <destination-dir>
```

Where the arguments are:
- -c, --conn-string : azure device connection string containing hostname, device ID and share access key
- -d, --directory   : destination directory where the developement environment will be generated

Note that, in order to simulate the module, you need the connection string which can be taken at the [Infrastructure Setup](#) page of the containerized sensor.

4. Build the docker image, using the dockerfiles present in the module's directory.

### Manual setup

1. Create a new development module in the `dev` directory. Select a module name and a template (e.g. c|csharp|java|nodejs|python|csharpfunction)

```bash
mkdir dev/<dest-dir> && cd dev/<dest-dir>
iotedgedev new -m <module-name> -t <template> .
```

2. Build the development module

```bash
iotedgedev build
```

3. Setup the IoT Edge device. For this you will need the connection string which can be provided in Huxon at the infrastructure setup step, where the various local nodes are defined. This step requires the sudo permissions

```bash
sudo iotedgehubdev setup -c "<connection-string>" -g edgehub
```

4. Run the simulation of the various edge modules to see the various messages being generated by the module. Be sure to use the correct architecture type, depending on the target architecture of the node that the module will be run.

```bash
iotedgehubdev start -d config/deployment.amd64.json -v
```

5. Build the docker image, using the dockerfiles present in the module's directory.

Now that everything is setup, you can modify and customize the module with your code, for instance using the code in the provided examples.

Once you change and modify the module run the following steps to re-build and re-deploy it (from the root directory of the module):

```bash
iotedgedev build
iotedgehubdev stop
iotedgehubdev start -d config/deployment.amd64.json -v
```
