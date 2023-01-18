ARG ARCH_TYPE=amd64

FROM ${ARCH_TYPE}/ubuntu:jammy

WORKDIR /app

RUN apt-get update && \
    apt-get install -y --no-install-recommends npm && \
    rm -rf /var/lib/apt/lists/*

COPY package.json ./
RUN npm install

COPY . .

RUN useradd -ms /bin/bash moduleuser
USER moduleuser

CMD [ "node", "main.js" ]