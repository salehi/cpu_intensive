# Extreme CPU Benchmark - Infrastructure Performance Testing

This image provides a CPU-intensive benchmark designed to stress-test your infrastructure and measure real-world computational performance. Perfect for comparing performance between bare metal, virtual machines, containers, and cloud environments.

## 🎯 Purpose

This benchmark aims to reveal the true performance characteristics of your infrastructure by executing billions of floating-point operations. It's particularly effective at exposing:

- Virtualization overhead in cloud environments
- CPU security mitigation impacts (Meltdown/Spectre)
- Container performance penalties
- Sustained CPU performance under load
- Infrastructure performance variations

## 🧮 What It Does

The benchmark performs intensive mathematical computations including:
- Custom Newton's method square root calculations
- Power functions, logarithms, and exponentials  
- Trigonometric operations (sin, cos, atan2)
- Complex floating-point arithmetic chains
- Memory allocation and random number generation

Each calculation is repeated thousands of times to create sustained CPU load, generating billions of floating-point operations that stress both CPU execution units and memory subsystems.

## 🚀 Quick Start

```bash
# Run with default settings (10M calculations, ~30-60 seconds)
docker run --rm your-username/cpu-benchmark

# Lighter test (faster completion)
docker run --rm -e NUM_CALCULATIONS=1000000 -e ITERATIONS_PER_NUMBER=1 s4l3h1/cpu_intensive

# Heavy stress test 
docker run --rm -e NUM_CALCULATIONS=10000000 -e ITERATIONS_PER_NUMBER=100 s4l3h1/cpu_intensive
