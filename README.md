# Huxon Containerized Sensor Examples

The [Huxon platform](https://huxon.huxelerate.it/docs/index.html) allows to define nodes that include the so-called **Containerized Sensors**.

A **Containerized Sensor** is an element in the infrastructure capable of producing data, similarly to a sensor of a [Sensor Board](https://huxon.huxelerate.it/docs/infrastructure_setup.html#sensor-boards-setup).

Each **containerized sensor** has a selected image that can be deployed and run in a docker container, producing the data, making it available in the user code, as well as to the
rest of the infrastructure nodes involved in the deployment.

The images are none other than Azure IoT Hub modules that can be deployed on an device, therefore we will refer to them as **modules** from now on.

The **containerized sensor** is useful when you want to import any existing code that produces data, inside the Huxon platform.

Please, in addition to the tutorial in this repository, refer also to the guides provided by Azure to understand how to construct and build the module using different languages: 

- [C](https://learn.microsoft.com/en-us/azure/iot-edge/tutorial-c-module?view=iotedge-2020-11>)
- [C#](https://learn.microsoft.com/en-us/azure/iot-edge/tutorial-csharp-module?view=iotedge-2020-11)
- [Java](https://learn.microsoft.com/en-us/azure/iot-edge/tutorial-java-module?view=iotedge-2020-11)
- [Node.js](https://learn.microsoft.com/en-us/azure/iot-edge/tutorial-node-module?view=iotedge-2020-11)
- [Python](https://learn.microsoft.com/en-us/azure/iot-edge/tutorial-python-module?view=iotedge-2020-11)

## Prerequisites

In order to test and use the containerized sensors, the following prerequisites must be met:

- [Huxon account](https://huxon.huxelerate.it/) to access the Huxon platform
- [Docker](https://docs.docker.com/get-docker/) to build the images for the Azure modules
- python to create and run the IoT edge modules
- npm to create NodeJS-based modules

To develop the modules and test them locally please run the following from the root directory of this repository, to correctly setup your system:

### Linux/MacOS

```bash
# Installs the python dependencies to use Azure IoT
python3 -m pip install iotedgedev

# In case you want to create a python-based module
python3 -m pip install cookiecutter

# In case you want to create a nodejs-based module
npm install -g yo # Yeoman
npm install -g generator-azure-iot-edge-module
```

## Custom Module

A module is made of two main components.

1. **Azure IoT interface**: this is the part that makes use of the libraries necessary to communicate using the MQTT protocol, allowing to connect the module with the rest of the IoT network;
2. **User custom code**: this is the part that the user will mainly develop and responsible for producing the data

The examples in this repository provide a skeleton mainly of the first part, showing how to use the tools provided by Azure IoT to create a client and start sending messages using the MQTT protocol.

All the low-level steps to handle the MQTT communication are handled by the Azure libraries.

To develop your own module then, you will need to mainly focus on the actual part that is responsible for producing/gathering the data from your local setup.

The examples provided are currently developed in the following languages:

- [C](examples/c)
- [Python](examples/python)
- [NodeJS](examples/nodejs)

## Module Development

You can develop your own module with the aid of the Azure IoT libraries, which can be used to:

- Create a new module
- Customize the module
- Test the module locally to verify its correct behavior
- Build the module and generate the docker

### Create

With this step you can generate a new empty module, based on one of the preferred supported languages (e.g. c|csharp|java|nodejs|python|csharpfunction).

```bash
mkdir dev/custom-module && cd dev/custom-module
iotedgedev new -m temperature -t python .
```

The code above creates a new directory called **custom-module** and runs the **temperature** module creation using the **python** template.

### Customize

The ``iotedgedev new`` command only generates a skeleton code with a dummy example that can be tailored based on your needs.

At this stage you can develop your own module logic and use the provided functions, in the respective chosen language, to initiate and communicate with the rest of the IoT infrastructure, making use of the Azure IoT low-level libraries.

### Build

Once you are ready to test your module, you need to run the build step, which generates a docker image that can be deployed on an IoT edge node, or simulated.

```bash
iotedgedev build
```
### Test

With the module built at the previous step, you can proceed with the simulation step.

To run the simulation you will first need the **connection string** which can be provided in the Huxon platform at the [infrastructure setup](https://huxon.huxelerate.it/docs/infrastructure_setup.html) step, where the various local nodes are defined.

The command below requires admin permissions, therefore it requires ``sudo`` permissions.

```bash
sudo iotedgehubdev setup -c "<connection-string>" -g edgehub
```

Now you can run the simulation of your module. Be sure to use the correct architecture type (e.g. amd64, arm32v7, etc.), depending on the target architecture of the node that the module will be run.

```bash
iotedgehubdev start -d config/deployment.amd64.json -v
```

### Iterate

The development step comes with several iteration of the Customize, Build and Test cycle, and each time that you start a new iteration, you want to test a new implementation, be sure to run all the steps once again, as the docker requires to be rebuilt, or otherwise you would be testing an outdated version of your module.

## Automatic setup

There is also a set of scripts available to make the above steps automatic.

### Create

```bash

# Arguments:
#   -t, --template: template type representing the language used to implement the module (c, csharp, java, nodejs, python, csharpfunction)
#   -m, --module: module name
#   -d, --directory: destination directory where the developement environment will be generated

bash scripts/create_dev_module.sh -m <module-name> -t <template> -d <destination-dir>
```

### Build and Simulate 

As previously, once the module is complete, you can build and test it locally, by running the following script:

```bash

# Arguments:
#   -c, --conn-string: azure device connection string containing hostname, device ID and share access key
#   -d, --directory: destination directory where the developement environment will be generated

bash scripts/run_dev_module.sh -c <connection-string> -d <destination-dir>
```

## Build Examples

To build the example docker images provided in this repository, you may run the following:

```bash
cd examples/<preferred language>
docker build -t <name-of-the-sensor> -f Dockerfile --build-arg ARCH_TYPE=<architecture-type> . 
```

Note that the architecture type can be provided as a build argument, and it needs to match the target architecture, where the module will run. E.g. a raspberry is based on ARM, therefore ARCH_TYPE needs to be `arm32v7`.

You may now push the docker image to your private/public registry and use it within the [Huxon platform](https://huxon.huxelerate.it/), when defining the corresponding containerized sensor.