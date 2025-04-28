FROM --platform=linux/amd64 ubuntu:22.04 as build

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Etc/UTC

RUN apt-get update && apt-get install rsync ninja-build build-essential cmake gcc-11 g++-11 libssl-dev wget -y

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 110 --slave /usr/bin/g++ g++ /usr/bin/g++-11

RUN wget https://archives.boost.io/release/1.79.0/source/boost_1_79_0.tar.gz && \
tar -xzf boost_1_79_0.tar.gz && \
cd boost_1_79_0 && ./bootstrap.sh && ./b2 install

RUN wget https://github.com/openssl/openssl/releases/download/openssl-3.4.0/openssl-3.4.0.tar.gz && \
tar -xf openssl-3.4.0.tar.gz && cd openssl-3.4.0 && \
./config --prefix=/opt/openssl-3.0 --openssldir=/opt/openssl-3.0 shared && \
make -j$(nproc) && make install

WORKDIR /home/authentication/

COPY . /home/authentication/

ENV OPENSSL_ROOT_DIR="/opt/openssl-3.0"
ENV OPENSSL_SSL_LIBRARY="${OPENSSL_ROOT_DIR}/lib64/libssl.so"
ENV OPENSSL_CRYPTO_LIBRARY="${OPENSSL_ROOT_DIR}/lib64/libcrypto.so"

RUN mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release \
          -DOPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR} \
          -DOPENSSL_SSL_LIBRARY=${OPENSSL_SSL_LIBRARY} \
          -DOPENSSL_CRYPTO_LIBRARY=${OPENSSL_CRYPTO_LIBRARY} .. && \
    cmake --build .

FROM --platform=linux/amd64 ubuntu:22.04

WORKDIR /home/neotek/authentication/

COPY --from=build /home/authentication/build/AuthenticationService /home/authentication/

CMD ["/home/authentication/AuthenticationService"]