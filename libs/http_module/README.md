# Http Module Documentation

The Http module implements HTTP/1.1 protocol.

## Module Type

The Http module implements the following module interfaces:
- `INetworkModule`

## Configuration

This section defines the configuration values the module depends on.

| Key                | Type | Required | Description                            |
|--------------------|------|----------|----------------------------------------|
 | `modules.http.port` |`int`| Yes      | Determines the port of the TCP socket. |
## Context

This section defines the request/response context values the module depends on.

| Key | Type                                        | Actions      | Description                                    |
|-----|---------------------------------------------|--------------|------------------------------------------------|
 |`_client`| `std::unique_ptr<network::tcp::ITCPClient>` | Read + Write | A pointer to the client which sent the request | 
 | `REMOTE_ADDR` | `std::uint32_t`                             | Write        | The client's ip address                        |
 | `PORT` | `std::uint16_t`                             | Write | The client's port| 
## Additional Comments
