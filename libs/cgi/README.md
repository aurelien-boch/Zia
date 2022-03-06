# Cgi Module Documentation

This module implements a generic CGI.

## Module Type

The Cgi module implements the following module interfaces:
 - `IHandlerModule`

## Configuration

This section defines the configuration values the module depends on.

| Key             | Type          | Required | Description               |
|-----------------|---------------|----------|---------------------------|
| `cgiPath`       | `std::string` | Yes      | Path to the cgi binary    |
| `rootDirectory` | `std::string` | Yes      | Root directory of modules |

## Context

This section defines the request/response context values the module depends on.

| Key           | Type            | Actions | Description           |
|---------------|-----------------|---------|-----------------------|
| `PORT`        | `std::uint16_t` | Read    | Port of the client    |
| `REMOTE_ADDR` | `std::uint32_t` | Read    | Address of the client |

---
[Back to README](../../README.md)
