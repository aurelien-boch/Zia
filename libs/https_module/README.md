# Https Module Documentation

The Https module implements HTTP/1.1 protocol with SSL.

## Module Type

The Https module implements the following module interfaces:
- `INetworkModule`

## Configuration

This section defines the configuration values the module depends on.

| Key                                   | Type         | Required | Description                                       |
|---------------------------------------|--------------|----------|---------------------------------------------------|
| `modules.https.port`                  | `int`        | Yes      | Determines the port of the TCP socket.            |
 | `modules.https.certificatePath`       | std::string  | Yes      | Determines the path to the SSL Certificate.       |
 | `modules.https.certificateKey`        | std::string  | Yes      | Determines the path to the Certificate's key      |
 | `modules.https.certificateDhFile`     | std::string  | Yes      | Determines the path to the Certificate's dh file  |
 | `modules.https.certificateKeyPassword`| std::sting   | Yes      | Determines the password for the Certificate's key |
## Context

This section defines the request/response context values the module depends on.

| Key           | Type                                        | Actions      | Description                                    |
|---------------|---------------------------------------------|--------------|------------------------------------------------|
| `_client`     | `std::unique_ptr<network::tcp::ITCPClient>` | Read + Write | A pointer to the client which sent the request | 
| `REMOTE_ADDR` | `std::uint32_t`                             | Write        | The client's ip address                        |
| `PORT`        | `std::uint16_t`                             | Write        | The client's port                              | 

---
[Back to README](../../README.md)
