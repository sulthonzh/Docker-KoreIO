FROM ubuntu:16.04
MAINTAINER info@intermediaware.de

RUN apt-get update
RUN apt-get -y install git
RUN apt-get -y install openssl libssl-dev
RUN apt-get -y install build-essential
RUN apt-get -y install vim
RUN apt-get -y install curl

WORKDIR /
RUN git clone https://github.com/jorisvink/kore.git
WORKDIR /kore

RUN make NOTLS=1 DEBUG=1 NOOPT=1
RUN make install NOTLS=1 DEBUG=1 NOOPT=1

WORKDIR /
ADD ./my_app /my_app

WORKDIR /my_app

EXPOSE 8888

CMD ["kodev", "run"]

