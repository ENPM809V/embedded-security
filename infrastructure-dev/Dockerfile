FROM ubuntu:18.04

RUN apt-get update && apt-get -y dist-upgrade && \
    apt-get install -y lib32z1 xinetd git libglib2.0-dev libfdt-dev libpixman-1-dev zlib1g-dev libnfs-dev libiscsi-dev build-essential ninja-build

RUN useradd -m ctf

WORKDIR /home/ctf

RUN cp -R /lib* /home/ctf && \
    cp -R /usr/lib* /home/ctf

RUN mkdir /home/ctf/dev && \
    mknod /home/ctf/dev/null c 1 3 && \
    mknod /home/ctf/dev/zero c 1 5 && \
    mknod /home/ctf/dev/random c 1 8 && \
    mknod /home/ctf/dev/urandom c 1 9 && \
    chmod 666 /home/ctf/dev/*

RUN cd /home/ctf/ && \
    git clone https://gitlab.com/qemu-project/qemu.git && \
    cd /home/ctf/qemu && \
    mkdir build && cd build && \
    ../configure --target-list=aarch64-softmmu,arm-linux-user && \
    make && make install

#RUN mkdir /home/ctf/libs
#COPY ./libpixman-1.so /home/ctf/libs/
#COPY ./libpixman-1.so.0 /home/ctf/libs
#COPY ./qemu-system-arm /home/ctf/libs

RUN mkdir /home/ctf/bin && \
    cp /bin/sh /home/ctf/bin && \
    cp /bin/ls /home/ctf/bin && \
    cp /bin/cat /home/ctf/bin

COPY ./ctf.xinetd /etc/xinetd.d/ctf
COPY ./start.sh /start.sh
RUN echo "Blocked by ctf_xinetd" > /etc/banner_fail

RUN chmod +x /start.sh

COPY ./bin/ /home/ctf/
RUN chown root:ctf /home/ctf && \
    chmod 750 /home/ctf && \
    chmod 740 /home/ctf/flag


ENV PATH="/home/ctf/qemu/build:${PATH}"

CMD ["/start.sh"]

EXPOSE 9999
EXPOSE 5900
