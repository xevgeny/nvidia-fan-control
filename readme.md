# nvidia-fan-control

A tiny C program to control fan speed of NVIDIA GPU

- custom fan curve
- works with both X.org and Wayland
- graceful shutdown on system signals
- restores fan control policy to NVML_FAN_POLICY_TEMPERATURE_CONTINOUS_SW (default) on exit

See example.cfg for more information.

## Requirements

- NVML: https://developer.nvidia.com/management-library-nvml
- libconfig

## Building and running tests

- `make` to build the program
- `make test` to run the tests 

## Usage

`./nvidia-fan-control example.cfg` to start the program. If you'd like run it as a systemd service, see [service.md](service.md).

## Troubleshooting

- if you get insufficient permissions error, try to run it as root
- on some GPUs setting fan speed below 30% will take no effect