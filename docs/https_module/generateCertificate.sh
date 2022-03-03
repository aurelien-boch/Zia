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
openssl genrsa -des3 -out server.key 1024
openssl req -new -key server.key -out server.csr
openssl x509 -req -days 3650 -in server.csr -signkey server.key -out server.crt
cp server.key server.key.secure
cp server.key server.key.secure
openssl dhparam -out dh2048.pem 2048
