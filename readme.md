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

2. Uninstall:
```
sudo make uninstall
```

By default, the program will be installed to `/usr/local/bin` and the config file will be copied to `/etc/nvidia-fan-control/config.cfg`.

## Troubleshooting

- If you get insufficient permissions error, try to run it as root
- On some GPUs setting fan speed below 30% will take no effect