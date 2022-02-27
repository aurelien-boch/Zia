## Usage

It doesn't require anything.

## 

# RequestReceptionTime Module Documentation

This module retrieves the time at which a request was received.

It is intended to be executed first in order to take into account the time of every single module executed.

## Module Type

The RequestReceptionTime module implements the following module interfaces:
- `IPreProcessorModule`

## Configuration

This section defines the configuration values the module depends on.

| Key | Type | Required | Description |
|-----|------|----------|-------------|

## Context

This section defines the request/response context values the module depends on.

| Key           | Type                                                 | Actions | Description                                     |
|---------------|------------------------------------------------------|---------|-------------------------------------------------|
| `received_at` | `std::chrono::time_point<std::chrono::system_clock>` | Write   | The time at which the request has been received |

## Additional Comments
