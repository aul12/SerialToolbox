FROM ubuntu:18.04

RUN apt-get update -y && apt-get install -y software-properties-common
RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test && apt-get update -y
RUN apt-get install -y
        libgtest-dev cmake valgrind gcc-8 g++-8 libasan5 qttools5-dev qttools5-dev-tools qtdeclarative5-dev google-mock
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 800 --slave /usr/bin/g++ g++ /usr/bin/g++-8
WORKDIR /usr/src/gtest
RUN cmake CMakeLists.txt
RUN make
RUN cp *.a /usr/lib
WORKDIR /usr/src/gmock
RUN cmake CMakeLists.txt
RUN make
RUN cp *.a /usr/lib
RUN ldconfig

RUN mkdir /src
COPY . /src/
RUN rm -rf /src/build
RUN mkdir -p /src/build

WORKDIR /src/build

RUN cmake -DCMAKE_BUILD_TYPE=Release .. && make -j2

CMD ["Tests/Tests", "--gtest_repeat=10", "--gtest_shuffle", "--gtest_color=yes"]

