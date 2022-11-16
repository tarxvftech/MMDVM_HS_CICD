from alpine
RUN apk add newlib-arm-none-eabi binutils-arm-none-eabi gcc-arm-none-eabi git make 
workdir /builder
add . /builder/ 
RUN git submodule init && git submodule update
VOLUME /out/
CMD ash buildall.sh
# docker build . -t mmdvm_hs_builder
# docker run --rm -it -v $PWD/out:/out mmdvm_hs_builder
#ash buildall.sh
