/**
 * @file sycl_aer.cpp
 * @brief Example of using a simulated QPU with SYCL and Qiskit Aer integration.
 * This example demonstrates how to create a SYCL queue for a simulated QPU,
 * apply various quantum gates to a quantum circuit, and measure the qubits.
 * The code is designed to run on both a simulated QPU and a CPU for testing
purposes.
 */

#include <sycl/sycl.hpp>

#include "sycl/detail/aer_simulator/quiskit_circuit.hpp"
#include <iostream>
#include <pybind11/embed.h>

int main() {
  pybind11::initialize_interpreter(); // Initialize the Python interpreter
  try {
    // Use a CPU to check that it fails
    sycl::queue q(sycl::cpu_selector_v);
    auto device = q.get_device();

    const int num_qubits = 3; // Number of qubits
    // Create a QuantumCircuit with 3 qubits and 3 classical bits
    py::object quantumCircuit =
        sycl::detail::createQuantumCircuit(num_qubits, device);

    // Initialize qubits to the desired state
    sycl::detail::initialize_qubits(quantumCircuit, "101");

    // Apply gates
    sycl::detail::x(quantumCircuit, 0);
    sycl::detail::y(quantumCircuit, 1);
    sycl::detail::z(quantumCircuit, 2);
    // Measure the qubits
    for (int i = 0; i < num_qubits; ++i) {
      sycl::detail::measure(quantumCircuit, i, i);
    }

    // Empty noise model
    nlohmann::json noise_model_json = {{"noise_model", {}},
                                       {"basis_gates", {"x", "y", "z"}}};

    // Execution configuration
    config::RunConfig run_config;
    run_config.shots = 1024;
    run_config.method =
        "statevector"; // You can also use "stabilizer", "density_matrix", etc.
    run_config.memory_slots = num_qubits;
    run_config.seed = 123;

    // Execute the quantum circuit
    nlohmann::json result = sycl::detail::executeQuantumCircuit(
        quantumCircuit, noise_model_json, run_config);

    // Show the complete result
    std::cout << "=== Resultado completo del simulador ===\n"
              << result.dump(4) << "\n";

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  try {
    // Use a simulated QPU to run the quantum circuit
    sycl::queue q(sycl::qpu_selector_v);
    auto device = q.get_device();

    const int num_qubits = 3; // Numero de qubits
    // Create a QuantumCircuit with 3 qubits and 3 classical bits
    py::object quantumCircuit =
        sycl::detail::createQuantumCircuit(num_qubits, device);

    // Initialize qubits to the desired state
    sycl::detail::initialize_qubits(quantumCircuit, "101");

    // Apply all possible gates
    sycl::detail::x(quantumCircuit, 0);
    sycl::detail::y(quantumCircuit, 1);
    sycl::detail::z(quantumCircuit, 2);

    sycl::detail::h(quantumCircuit, 0);
    sycl::detail::rx(quantumCircuit, 1, 3.14159 / 2); // π/2
    sycl::detail::ry(quantumCircuit, 2, 3.14159 / 4); // π/4
    sycl::detail::rz(quantumCircuit, 0, 3.14159);     // π

    sycl::detail::s(quantumCircuit, 1);
    sycl::detail::sdg(quantumCircuit, 2);
    sycl::detail::t(quantumCircuit, 0);
    sycl::detail::tdg(quantumCircuit, 1);

    sycl::detail::cx(quantumCircuit, 0, 1);

    // Measure the qubits
    for (int i = 0; i < num_qubits; ++i) {
      sycl::detail::measure(quantumCircuit, i, i);
    }

    // Empty noise model
    nlohmann::json noise_model_json = {
        {"noise_model", {}},
        {"basis_gates",
         {"x", "y", "z", "h", "rx", "ry", "rz", "s", "sdg", "t", "tdg", "cx"}}};

    // Execution configuration
    config::RunConfig run_config;
    run_config.shots = 1024;
    run_config.method =
        "statevector"; // You can also use "stabilizer", "density_matrix", etc.
    run_config.memory_slots = num_qubits;
    run_config.seed = 123;

    // Draw the circuit diagram
    sycl::detail::print_circuit_diagram(quantumCircuit);

    // Execute the quantum circuit
    nlohmann::json result = sycl::detail::executeQuantumCircuit(
        quantumCircuit, noise_model_json, run_config);

    // Show the complete result
    std::cout << "=== Complete results ===\n" << result.dump(4) << "\n";

    // Show the counts in binary format
    auto counts = result["results"][0]["data"]["counts"];
    std::cout << "\n=== Counts ===\n";
    for (auto &[state_hex, count] : counts.items()) {
      // Convert hexadecimal state to binary
      unsigned int state_int = std::stoul(state_hex, nullptr, 16);
      std::string binary =
          std::bitset<num_qubits>(state_int).to_string(); // 3 qubits
      std::cout << binary << " => " << count << "\n";
    }

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  pybind11::finalize_interpreter(); // Close the Python interpreter
  return 0;
}

/**
 * EXPECTED OUTPUT:
    Error: This function can only be used with a QPU simulated device.
    === Complete results ===
    {
        "backend_name": "",
        "backend_version": "",
        "date": "",
        "job_id": "",
        "metadata": {
            "max_gpu_memory_mb": 0,
            "max_memory_mb": 7630,
            "omp_enabled": false,
            "parallel_experiments": 1,
            "time_taken_execute": 0.009685701,
            "time_taken_parameter_binding": 4.2634e-05
        },
        "qobj_id": "",
        "results": [
            {
                "data": {
                    "counts": {
                        "0x0": 23,
                        "0x1": 38,
                        "0x2": 36,
                        "0x3": 40,
                        "0x4": 202,
                        "0x5": 214,
                        "0x6": 246,
                        "0x7": 225
                    }
                },
                "metadata": {
                    "active_input_qubits": [
                        0,
                        1,
                        2
                    ],
                    "batched_shots_optimization": false,
                    "device": "CPU",
                    "fusion": {
                        "applied": false,
                        "enabled": true,
                        "max_fused_qubits": 5,
                        "threshold": 14
                    },
                    "input_qubit_map": [
                        [
                            2,
                            2
                        ],
                        [
                            1,
                            1
                        ],
                        [
                            0,
                            0
                        ]
                    ],
                    "max_memory_mb": 7630,
                    "measure_sampling": true,
                    "method": "statevector",
                    "noise": "ideal",
                    "num_bind_params": 1,
                    "num_clbits": 3,
                    "num_qubits": 3,
                    "parallel_shots": 1,
                    "parallel_state_update": 1,
                    "remapped_qubits": false,
                    "required_memory_mb": 1,
                    "runtime_parameter_bind": false,
                    "sample_measure_time": 0.001087288,
                    "time_taken": 0.009608535
                },
                "seed_simulator": 123,
                "shots": 1024,
                "status": "DONE",
                "success": true,
                "time_taken": 0.009608535
            }
        ],
        "status": "COMPLETED",
        "success": true
    }

    === Counts ===
    000 => 23
    001 => 38
    010 => 36
    011 => 40
    100 => 202
    101 => 214
    110 => 246
    111 => 225
 */