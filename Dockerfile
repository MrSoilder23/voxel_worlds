FROM ubuntu:25.04
WORKDIR /app

RUN apt-get update

RUN apt-get install -y \
    software-properties-common wget gpg \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update 
RUN apt-get install -y --no-install-recommends \
    build-essential \
    git cmake ninja-build \
    pkg-config curl \
    libtool \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update 

RUN apt-get install -y \
    libltdl-dev python3-pip

RUN pip3 install Jinja2 --break-system-packages

COPY . .

RUN cmake -S . -B build -G Ninja

RUN cmake --build build --config Release