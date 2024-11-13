#!/bin/bash

ls -la .
realpath .
export PATH="$(realpath ./bin/python3/):$(realpath ./bin/rust/):$PATH"
./CodeSandbox
