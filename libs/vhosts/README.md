# Vhosts Module Documentation

Vhosts module allows you to create simple vhosts

## Module Type

The Vhosts module implements the following module interfaces:
 - Vhosts

## Configuration

This section defines the configuration values the module depends on.
The module will just scan the configuration and assign each key of the vhosts module
to a vhost, and it's value to the target ip and port.
format: vhost: target.ip:port
example:

zia-php-cgi.epitech: 127.0.0.1:8000

---
[Back to README](../../README.md)
