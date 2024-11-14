#!/bin/bash

ls -la .
realpath .
export PATH="$(realpath ./bin/python3/bin/):$(realpath ./bin/rust/bin/):$PATH"
python3.13 --version
rustc --version
./CodeSandbox
