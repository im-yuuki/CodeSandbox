FROM ubuntu:latest
ENV DEBIAN_FRONTEND=noninteractive

# Toolchain
RUN apt-get update
RUN apt-get install -y gcc g++
RUN rm -rf /var/lib/apt/lists/*
COPY bin /toolchain
ENV PATH="/toolchain/python3/bin/:/toolchain/rust/bin/:$PATH" 

# App
WORKDIR /app
COPY build/CodeSandbox .
EXPOSE 4000/tcp

# Run in executables form, to avoid starting unnecessary processes
RUN chmod +x CodeSandbox
CMD ["./CodeSandbox"]