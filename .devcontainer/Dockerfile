# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.217.4/containers/cpp/.devcontainer/base.Dockerfile

# [Choice] Debian / Ubuntu version (use Debian 11, Ubuntu 18.04/21.04 on local arm64/Apple Silicon): debian-11, debian-10, ubuntu-21.04, ubuntu-20.04, ubuntu-18.04
ARG VARIANT="bullseye"
FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}

# [Optional] Uncomment this section to install additional packages.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends           \
    python3-pip                                             \
    build-essential                                         \
    wget                                                    \
    cmake                                                   \
    gcovr                                                   \
    clang-format

# Install conan
RUN pip3 install conan
RUN pip3 install conan --upgrade

# Install fish
RUN echo 'deb http://download.opensuse.org/repositories/shells:/fish:/release:/3/Debian_10/ /' | sudo tee /etc/apt/sources.list.d/shells:fish:release:3.list
RUN curl -fsSL https://download.opensuse.org/repositories/shells:fish:release:3/Debian_10/Release.key | gpg --dearmor | sudo tee /etc/apt/trusted.gpg.d/shells_fish_release_3.gpg > /dev/null
RUN sudo apt update
RUN sudo apt install --no-install-recommends -y fish

# Use the vscode user profile
USER vscode
