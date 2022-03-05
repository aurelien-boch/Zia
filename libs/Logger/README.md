# Logger Module Documentation

Logs the incoming requests and the response status to the console.

## Module Type

The Logger module implements the following module interfaces:
- `IPostProcessorModule`

## Configuration

This section defines the configuration values the module depends on.

| Key            | Type          | Required | Description              |
|----------------|---------------|----------|--------------------------|
| `pipelineName` | `std::string` | Yes      | The name of the pipeline |

## Context

This section defines the request/response context values the module depends on.

| Key           | Type                                                 | Actions | Description                                                                                 |
|---------------|------------------------------------------------------|---------|---------------------------------------------------------------------------------------------|
| `received_at` | `std::chrono::time_point<std::chrono::system_clock>` | Read    | The time at which the request was <br/>received (`RequestReceptionTime` Module provides it) |
| `REMOTE_ADDR` | `std::uint32_t`                                      | Read    | The IP Address of the client                                                                |

---
[Back to README](../../README.md)