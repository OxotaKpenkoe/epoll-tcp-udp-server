# Epoll TCP/UDP Server

Asynchronous C++17 server for handling TCP and UDP connections using the epoll event multiplexer.

## Features

- Asynchronous client handling via epoll
- Support for TCP and UDP protocols
- Built-in commands:
  - `/time` - return current server time
  - `/stats` - show connection statistics
  - `/shutdown` - stop the server
- Echo mode for all non-command messages

## Requirements

- Ubuntu 24.04 or compatible Linux distribution
- g++ with C++17 support
- make
- systemd (optional, for running as a service)

## Build
```bash
make
```

## Run
```bash
# Default run (TCP: 38080, UDP: 38081)
make run

# Manual run
./build/simple-server <tcp_port> <udp_port>
```

## Usage Examples
```bash
# TCP:
echo "/stats" | nc 127.0.0.1 38080

# UDP:
echo "/stats" | nc -u 127.0.0.1 38081
```

## Installation

### Using .deb package:
```bash
# Build the package
make deb

# Install
sudo dpkg -i simple-server_1.0.0_amd64.deb

# Start service
sudo systemctl start simple-server
sudo systemctl status simple-server

# Enable auto-start
sudo systemctl enable simple-server

# Remove
sudo dpkg -r simple-server
```

### Manual installation
```bash
# Copy binary
sudo cp build/simple-server /usr/local/bin/

# Copy systemd unit
sudo cp simple-server.service /etc/systemd/system/

# Start service
sudo systemctl daemon-reload
sudo systemctl start simple-server
sudo systemctl enable simple-server
```
