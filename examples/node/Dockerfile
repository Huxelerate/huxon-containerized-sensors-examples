FROM amd64/ubuntu:latest

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