# Huxon Containerized Sensor Examples

This repository contains a set of examples to create containerized sensors that can be uploaded and used in the Huxon platform, to provide data within the various deployed algorithms of your IoT solutions.

The examples can be coded in the following languages:

- C
- Python
- NodeJS

## Instructions

To correctly build the containers, run the following:

```bash
cd examples/<preferred language>
docker build -t <name of the sensor> -f Dockerfile .
```

You may now push the docker image to your private/public registry and use it within the Huxon platform, when defining the corresponding containerized sensor.
