# nvidia-fan-control

A tiny C program to control fan speeds of NVIDIA GPUs. Based on [NVML](https://developer.nvidia.com/management-library-nvml).

## Why This Exists

I had been using [GWE](https://gitlab.com/leinardi/gwe) to set custom fan curves for my GPU. Unfortunately, GWE uses NV-CONTROL which requires X.Org. The good thing about NVML that it works on both X and Wayland.

## Requirements

- NVML
- libconfig

## Building and Running Tests

- Run `make` to build the program
- Run `make test` to execute the tests 