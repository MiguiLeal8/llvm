/**
 * @file native_qpu.cpp
 * @brief Example of using  simulated QPU over a native CPU with SYCL.
 * This example demonstrates how you cannot apply quantum gates without a
 somulated QPU device.
 * It also includes a device selector to create the queue with a CPU.
 * The Hadamard gate is applied to a 2-qubit state vector.
 * The code is designed to run on both a simulated QPU and a CPU for testing
 purposes.
 */

#include <sycl/sycl.hpp>

int main() {
  try {
    // Task: add a device selector to create this queue with an Intel GPU

    auto platforms = sycl::platform::get_platforms();
    auto devices = sycl::device::get_devices();
    for (const auto &platform : platforms) {
      std::cout << "Platform: "
                << platform.get_info<sycl::info::platform::name>() << "\n";
      for (const auto &device : platform.get_devices()) {
        std::cout << "  Device: " << device.get_info<sycl::info::device::name>()
                  << "\n";
      }
    }
  } catch (const sycl::exception &e) {
    std::cout << "Exception caught: " << e.what() << std::endl;
  }
  try {
    sycl::queue q(sycl::qpu_selector_v); // Select simulated QPU
    std::complex<float> stateVector[4] = {
        {1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}};
    h(q, stateVector, 2, 0); // Apply Hadamard gate
    std::cout << "QPU function executed successfully." << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  try {
    sycl::queue q(sycl::cpu_selector_v); // Select CPU for testing
    std::complex<float> stateVector[4] = {
        {1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}};
    h(q, stateVector, 2, 0); // Apply Hadamard gate
    std::cout << "CPU function executed successfully." << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

/**
 * EXPECTED OUTPUT:
    Platform: SYCL_NATIVE_CPU
      Device: Native CPU
      Device: Simulated QPU
    QPU function executed successfully.
    Error: This function can only be used with a QPU simulated device.
 */