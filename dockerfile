FROM ubuntu:latest
RUN apt-get update -y
RUN apt-get install -y gcc g++
WORKDIR /app
COPY build/CodeSandbox .
# Python & Rust
COPY bin ./bin
COPY start.sh .
EXPOSE 4000/tcp
RUN chmod +x CodeSandbox
CMD ["./start.sh"]