FROM gcc:latest

RUN mkdir -p /usr/src/app/
WORKDIR /usr/src/app/
COPY . /usr/src/app/

RUN apt-get update && \
    apt-get install -y \
      libboost-dev libboost-program-options-dev \
      libgtest-dev \
      cmake

RUN cmake . -D TEST_MODE=FALSE && \
    make
