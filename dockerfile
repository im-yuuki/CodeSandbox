FROM ubuntu:latest
RUN apt update -y
RUN apt install -y gcc g++
WORKDIR /app
COPY build/CodeSandbox .
EXPOSE 4000/tcp
RUN chmod +x CodeSandbox
CMD ["./CodeSandbox"]