FROM amd64/ubuntu:jammy

WORKDIR /app

RUN apt-get update && \
    apt-get install -y --no-install-recommends python3 python3-pip libboost-python-dev && \
    rm -rf /var/lib/apt/lists/*

COPY requirements.txt ./
RUN python3 -m pip install -r requirements.txt

COPY . .

RUN useradd -ms /bin/bash moduleuser
USER moduleuser

CMD [ "python3", "-u", "./main.py" ]