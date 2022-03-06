# Https Module Documentation

The Https module implements HTTP/1.1 protocol with SSL.

## Module Type

The Https module implements the following module interfaces:
- `INetworkModule`

## Configuration

This section defines the configuration values the module depends on.

**Note:** If some file paths are missing from the configuration file or if it is not provided, the program will assume it is running from the `./build/bin/` directory. 

| Key                                     | Type            | Required | Description                                                                                         |
|-----------------------------------------|-----------------|----------|-----------------------------------------------------------------------------------------------------|
| `modules.https.port`                    | `std::uint16_t` |  No      | Determines the port of the TCP socket. It's default value is `443`                                  |
 | `modules.https.certificatePath`         | `std::string`   | No       | Determines the path to the SSL Certificate. It's default value is `../certificate/server.crt"`      |
 | `modules.https.certificateKey`          | `std::string`   | No       | Determines the path to the Certificate's key. It's default value is `../certificate/server.key`     |
 | `modules.https.certificateDhFile`       | `std::string`   | No       | Determines the path to the Certificate's dh file. It's default value is `../certificate/dh2048.pem` |
 | `modules.https.certificateKeyPassword`  | `std::string`   | No       | Determines the password for the Certificate's key. It's default value is `password`                 |
## Context

This section defines the request/response context values the module depends on.

| Key           | Type                                        | Actions      | Description                                    |
|---------------|---------------------------------------------|--------------|------------------------------------------------|
| `_client`     | `std::unique_ptr<network::tcp::ITCPClient>` | Read + Write | A pointer to the client which sent the request | 
| `REMOTE_ADDR` | `std::uint32_t`                             | Write        | The client's ip address                        |
| `PORT`        | `std::uint16_t`                             | Write        | The client's port                              | 

## Additional Comments

If the `port` value is lower than `1024` (like the default value), the program **MUST** run with administrator privileges.

---
[Back to README](../../README.md)
