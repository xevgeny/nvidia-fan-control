# nvidia-fan-control

A tiny C program to control fan speed of NVIDIA GPU

- custom fan curve
- works with both X.org and Wayland
- graceful shutdown on system signals
- restores fan control policy to NVML_FAN_POLICY_TEMPERATURE_CONTINOUS_SW (default) on exit

See [example.cfg](example.cfg) for more information.

## Requirements

- [NVML](https://developer.nvidia.com/management-library-nvml)
- libconfig

## Building and running tests

- `make` to build the program
- `make test` to run the tests 

## Usage

`build/nvidia-fan-control example.cfg` to start the program. If you'd like run it as a systemd service, see [service.md](service.md).

## Installation

1. Default installation:
```
sudo make install
```

2. Custom installation:
```
sudo make install PREFIX=/usr/local SYSCONFDIR=/etc
```

3. Uninstall:
```
sudo make uninstall
```

## Troubleshooting

- if you get insufficient permissions error, try to run it as root
- on some GPUs setting fan speed below 30% will take no effect