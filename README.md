# Huxon Containerized Sensor Examples

[Huxon platform](https://huxon.huxelerate.it/docs/index.html) allows to define nodes that include **Containerized Sensors**.

A **Containerized Sensor** is an element in the infrastructure capable of producing data, similarly to a sensor of a [Sensor Board](https://huxon.huxelerate.it/docs/infrastructure_setup.html#sensor-boards-setup).

The **Containerized Sensor** can have multiple images, but you have to define the selected image as the one that can be deployed and run in a docker container that produces data, making it available in the user code

Images are implemented as Azure IoT Hub modules, therefore, to comply with Azure naming, we will refer to them as **modules** from now on.

The **containerized sensor** is useful when you want to import any existing code that produces data, inside the Huxon platform. A full documentation on how to handle **containerized sensor** can be found on [Huxon platform](https://huxon.huxelerate.it/docs/infrastructure_setup.html#containerized-sensor).

In addition to the tutorial in this repository, refer also to the guides provided by Azure to better understand how to use modules: 

- [C](https://learn.microsoft.com/en-us/azure/iot-edge/tutorial-c-module?view=iotedge-2020-11>)
- [C#](https://learn.microsoft.com/en-us/azure/iot-edge/tutorial-csharp-module?view=iotedge-2020-11)
- [Java](https://learn.microsoft.com/en-us/azure/iot-edge/tutorial-java-module?view=iotedge-2020-11)
- [Node.js](https://learn.microsoft.com/en-us/azure/iot-edge/tutorial-node-module?view=iotedge-2020-11)
- [Python](https://learn.microsoft.com/en-us/azure/iot-edge/tutorial-python-module?view=iotedge-2020-11)

## Prerequisites

To test and use the containerized sensors, the following prerequisites must be met:

- **[Huxon account](https://huxon.huxelerate.it/)** to access the Huxon platform
- **[Docker](https://docs.docker.com/get-docker/)** to build the images for the Azure modules
- **python** to create and run the IoT edge modules
- **npm** to create NodeJS-based modules

To develop the modules and test them locally please run the following to setup your system:

#### Linux/MacOS

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

1. **Azure IoT interface**: libraries layer to communicate using the MQTT protocol, allowing to connect the module with the rest of the IoT network;
2. **User custom code**: user-defined code to produce and gather data.

The examples in this repository include code that shows how to use the Azure IoT library. It instantiates a client and starts sending messages using the MQTT protocol to a dedicated output channel.

As an excercise, you can customize the user-defined code to change the behavior of this example.

The examples provided are currently developed in the following languages:

- [C](examples/c)
- [Python](examples/python)
- [NodeJS](examples/nodejs)

## Module Development

The steps to develop your own module using Azure IoT libraries are:

- [Create](#create) a new module
- [Customize](#customize) the module
- [Build](#build) the module and generate the docker
- [Test](#test) the module locally to verify its correct behavior

#### Create

With this step you can generate a new empty module, based on one of the preferred supported languages (e.g. c|csharp|java|nodejs|python|csharpfunction).

```bash
mkdir dev/custom-module && cd dev/custom-module
iotedgedev new -m temperature -t python .
```

The code above creates a new directory called **custom-module** and runs the **temperature** module creation using the **python** template.

The result is a directory with a basic example and an initial code to begin with the developement and customization of the module.

#### Customize

At this stage you can develop your own module logic and use the provided functions to initiate and communicate with the rest of the IoT infrastructure, making use of the Azure IoT low-level libraries.

#### Build

Once you are ready to test your module, you first need to run the build step to a docker image that can be deployed on an IoT edge node, or simulated.

```bash
iotedgedev build
```
#### Test

With the built module, you can proceed with the simulation step.

To run the simulation you will first need the **connection string** which can be provided in the Huxon platform at the [infrastructure setup](https://huxon.huxelerate.it/docs/infrastructure_setup.html) step, where the various local nodes are defined.

The command below requires admin permissions, therefore it requires ``sudo`` permissions.

```bash
sudo iotedgehubdev setup -c "<connection-string>" -g edgehub
```

Now you can run the simulation of your module. Be sure to use the correct architecture type (e.g. amd64, arm32v7, etc.), depending on the target architecture of the node that the module will be run.

```bash
iotedgehubdev start -d config/deployment.amd64.json -v
```

#### Iterate

The development step comes with several iteration of the Customize, Build and Test cycle, and each time that you start a new iteration, you want to test a new implementation, be sure to run all the steps once again, as the docker requires to be rebuilt, or otherwise you would be testing an outdated version of your module.

### Automatic setup

There is also a set of scripts available to make the above steps automatic.

#### Create

```bash

# Arguments:
#   -t, --template: template type representing the language used to implement the module (c, csharp, java, nodejs, python, csharpfunction)
#   -m, --module: module name
#   -d, --directory: destination directory where the developement environment will be generated

bash scripts/create_dev_module.sh -m <module-name> -t <template> -d <destination-dir>
```

#### Build and Simulate 

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

#### Containerized Sensor for Huxon

To integrate a custom module within the Huxon platform as a Containerized Sensor, you need to make sure that the module outputs the data on the dedicated output channel, which is `sensoroutput`.