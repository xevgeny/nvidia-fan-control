# nvidia-fan-control

A tiny C program to control fan speed of NVIDIA GPUs

## Why This Exists

I had been using [GWE](https://gitlab.com/leinardi/gwe) to set custom fan curve for my GPU. Unfortunately, GWE uses NV-CONTROL which requires X.org. The good thing about NVML that it works with both X.org and Wayland.

## Requirements

- [NVML](https://developer.nvidia.com/management-library-nvml)
- libconfig

## Building and Running Tests

- `make` to build the program
- `make test` to run the tests 