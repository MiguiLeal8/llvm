/**
 * @file native_qpu_cluster.cpp
 * @brief Example of using a simulated QPU with SYCL.
 * This example demonstrates how to create a SYCL queue for a simulated QPU,
 * apply a Hadamard gate to a quantum state vector, and measure the state.
 * The Hadamard gate is applied to a 3-qubit state vector.
 */

#include <sycl/sycl.hpp>

int main() {
  try {
    sycl::queue q(sycl::qpu_selector_v); // Select simulated QPU

    constexpr size_t numQubits = 3;
    constexpr size_t numStates = 1 << numQubits; // 2^n
    std::complex<float> *stateVector = new std::complex<float>[numStates];
    std::complex<float> *stateVector_d =
        malloc_device<std::complex<float>>(numStates, q);

    // Initial state
    stateVector[0] = std::complex<float>(1.0f, 0.0f);
    q.memcpy(stateVector_d, stateVector,
             numStates * sizeof(std::complex<float>))
        .wait();

    for (int i = 0; i < numQubits; i++) {
      h(q, stateVector_d, numQubits, i);
    }
    measure(q, stateVector_d, numQubits, 1000);
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

/**
 * EXPECTED OUTPUT:
    Quantum State Probabilities:
    State 000: 128
    State 001: 119
    State 010: 125
    State 011: 119
    State 100: 120
    State 101: 140
    State 110: 120
    State 111: 129
 */