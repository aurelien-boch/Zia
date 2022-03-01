#!/bin/bash

echo "This script generates a Certificate in the build/certificate/ directory."
if [[ $PWD == *"docs/https_module"* ]]; then
    cd ../../
elif [[ $PWD == *"docs" ]]; then
    cd ..
fi

# Create build directory if doesn't exist
if [[ ! -d "build/" ]]; then
    echo "No build/ directory found... Creating one."
    mkdir build/
fi
cd build/

# Create certificate directory if does not exist
if [[ ! -d "certificate/" ]]; then
    mkdir certificate
fi
cd certificate/

# Certificate generation
openssl genrsa -aes256 -passout pass:gsahdg -out server.pass.key 4096
openssl rsa -passin pass:gsahdg -in server.pass.key -out server.key
rm server.pass.key
openssl req -new -key server.key -out server.csr
openssl x509 -req -sha256 -days 365 -in server.csr -signkey server.key -out server.crt
