ARG ARCH_TYPE=amd64

FROM ${ARCH_TYPE}/ubuntu:bionic AS base
RUN apt-get update && \
    apt-get install -y --no-install-recommends software-properties-common && \
    add-apt-repository -y ppa:aziotsdklinux/ppa-azureiot && \
    apt-get update && \
    apt-get install -y azure-iot-sdk-c-dev && \
    rm -rf /var/lib/apt/lists/*

FROM base AS build-env
RUN apt-get update && \
    apt-get install -y --no-install-recommends cmake gcc g++ make && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . ./
RUN cmake .
RUN make

FROM base
WORKDIR /app
COPY --from=build-env /app ./
RUN useradd -ms /bin/bash moduleuser
USER moduleuser
CMD ["./main"]
