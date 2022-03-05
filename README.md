# Zia

The purpose of the Zia project is to implement an Http server based on the [ZiAPI](https://github.com/martin-olivier/ZiAPI) api.

### Available modules

The server comes with several modules, including:
- a generic `CGI` module
- a `Logger` module
- an `Http` module implementing HTTP/1.1 protocol
- an `Https` module implementing HTTP/1.1 protocol with SSL

Note that all the modules based on the `ZiAPI` api (v5.0.0) are compatible with our Zia server. 

### Other features

The server also comes wit the following features:
- A `cli` to manage the server and the pipelines

## Build the project

Create a `build/` directory:
```shell
mkdir build/
```

### Conan

Install the required conan packages:

```shell
conan install . --build=missing -if=build/
```

### CMake

#### Tests
**If** you want to build the unit tests:

```shell
cmake -DENABLE_TESTING=yes ./build/
```

#### Project
Build the project with cmake:

```shell
cmake --build ./build/
```

All the binaries are located in the `./build/bin/` directory.

## Configuration

We choose the `Yaml` language for the configuration files.

### Pipelines configuration

The pipelines configurations must be written in the `pipelines.txt` file. It is formatted as follows:
```
pipelineName::pathToTheModulesConfigurationFile::PathsToTheLibraries
...
```

### Modules configuration

Each pipeline must provide a module configuration file, formatted as follows:

```yaml
modules:
  moduleName:
    moduleParam1: moduleParam1
    moduleParam2: moduleParam2
  ...
```

The modules parameters required in the configuration are documented in the [Modules documentation](#modules-documentation) 

## Run

Run the `zia` binary (located in the `./build/bin/` directory).

### Use the cli

The `cli` enables server and pipelines management. The following commands are available:

| Command    | Argument(s)    | Description                            |
|------------|----------------|----------------------------------------|
 | `help`     |                | Displays the commands list             |
 | `start`    | `pipelineName` | Starts the pipeline                    |
 | `stop`     | `pipelineName` | Stops the pipeline                     |
 | `config`   | `pipelineName` | Reloads the pipeline configuration     |
 | `hotreload`| `pipelineName` | Activates or deactivates hot reloading |

## Documentation

### Modules documentation

- [Cgi](./libs/cgi/README.md)
- [Logger](./libs/Logger/README.md)
- [HttpModule](./libs/http_module/README.md)
- [HttpsModule](./libs/https_module/README.md)

### Doxygen documentation

Generate the Doxygen documentation:
```shell
doxygen Doxyfile
```
All the documentation will be found in the `./doc/` directory.

## Notes

### SSL certificate generation

A script is provided to generate SSL certificates. Run:
```shell
./docs/https_module/generateCertificate.sh
```

All the required files will be provided in the `./build/certificate/` directory.

## Credits

[Aurélien BOCH](https://github.com/aurelien-boch) | <aurelien.boch@epitech.eu>

[Benjamin REIGNER](https://github.com/Breigner01) | <benjamin.reigner@epitech.eu>

[Paul BEAUDET](https://github.com/Pepiloto) | <paul.beaudet@epitech.eu>

[Victoire Rabeau](https://github.com/Victoire-Rabeau) | <victoire.rabeau@epitech.eu>