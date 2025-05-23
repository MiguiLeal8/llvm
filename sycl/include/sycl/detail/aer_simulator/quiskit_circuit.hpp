#pragma once

#include "sycl/detail/aer_simulator/aer_simulator.hpp"
#include <pybind11/embed.h> // pybind11 para interactuar con Python
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>
#include <vector>

#include <sycl/detail/qpu.hpp>

namespace py = pybind11;

namespace sycl {
inline namespace _V1 {
namespace detail {

// Crear un QuantumCircuit en Python
py::object createQuantumCircuit(int numQubits, const sycl::device &device) {
  // Verificar que el dispositivo sea una QPU
  ensure_qpu_device(device);

  py::module qiskit = py::module::import("qiskit");
  // Crear circuito con el mismo número de classical bits que qubits
  return qiskit.attr("QuantumCircuit")(numQubits, numQubits);
}

// Añadir una puerta Hadamard al QuantumCircuit
void h(py::object &quantumCircuit, int target) {
  quantumCircuit.attr("h")(target);
}

// Añadir una rotación en X al QuantumCircuit
void rx(py::object &quantumCircuit, int target, double angle) {
  quantumCircuit.attr("rx")(angle, target);
}

// Medir un qubit en el QuantumCircuit
void measure(py::object &quantumCircuit, int target, int classicalBit) {
  quantumCircuit.attr("measure")(target, classicalBit);
}

// Ejecutar el QuantumCircuit en el simulador
nlohmann::json executeQuantumCircuit(py::object &quantumCircuit,
                                     const nlohmann::json &noise_model_json,
                                     const config::RunConfig &run_config) {
  try {
    // Importar el módulo circuit.py
    py::module circuit = py::module::import(
        "sycl.include.sycl.detail.aer_simulator.cunqa.circuit");

    // Convertir el QuantumCircuit a JSON usando qc_to_json
    py::object json_dict = circuit.attr("qc_to_json")(quantumCircuit);

    // Convertir dict a string JSON en Python
    py::object json_str = py::module::import("json").attr("dumps")(json_dict);

    // Parsear string JSON en C++
    nlohmann::json circuit_json =
        nlohmann::json::parse(json_str.cast<std::string>());

    std::cout << circuit_json.dump(4) << std::endl;

    // Ejecutar el circuito utilizando AerSimulator
    return AerSimulator::execute(circuit_json, noise_model_json, run_config);
  } catch (const py::error_already_set &e) {
    throw std::runtime_error("Error al convertir o ejecutar el circuito: " +
                             std::string(e.what()));
  } catch (const std::exception &e) {
    throw std::runtime_error("Error en AerSimulator::execute: " +
                             std::string(e.what()));
  }
}

} // namespace detail
} // namespace _V1
} // namespace sycl