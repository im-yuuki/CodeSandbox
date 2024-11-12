FROM ubuntu:latest
RUN apt update -y
RUN apt install -y gcc g++
# Install rust from rustup (since we can't trust the version in the package manager)
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
RUN rustup install stable
RUN rustup default stable
WORKDIR /app
COPY build/CodeSandbox .
EXPOSE 4000/tcp
RUN chmod +x CodeSandbox
CMD ["./CodeSandbox"]