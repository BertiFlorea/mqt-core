#ifndef JKQZX_INCLUDE_GRAPH_HPP_
#define JKQZX_INCLUDE_GRAPH_HPP_

#include <algorithm>
#include <cstddef>
#include <numeric>
#include <optional>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "QuantumComputation.hpp"

#include "Definitions.hpp"
#include "Rational.hpp"
#include "Utils.hpp"
#include "dd/Definitions.hpp"

namespace zx {

class ZXDiagram {
public:
  ZXDiagram() = default;
  ZXDiagram(int32_t nqubits); // create n_qubit identity_diagram
  explicit ZXDiagram(std::string filename);
  explicit ZXDiagram(const qc::QuantumComputation& circuit);

  void add_edge(Vertex from, Vertex to, EdgeType type = EdgeType::Simple);
  void add_hadamard_edge(Vertex from, Vertex to) {
    add_edge(from, to, EdgeType::Hadamard);
  };
  void add_edge_parallel_aware(Vertex from, Vertex to,
                               EdgeType type = EdgeType::Simple);
  void remove_edge(Vertex from, Vertex to);

  Vertex add_vertex(const VertexData &data);
  Vertex add_vertex(dd::Qubit qubit, Col col = 0,
                    Rational phase = Rational(0, 1),
                    VertexType type = VertexType::Z);
  void remove_vertex(Vertex to_remove);

  int32_t get_ndeleted()const { return deleted.size();}
  [[nodiscard]] int32_t get_nvertices() const { return nvertices; }
  [[nodiscard]] int32_t get_nedges() const { return nedges; }
  [[nodiscard]] int32_t get_nqubits() const { return inputs.size(); }

  [[nodiscard]] bool connected(Vertex from, Vertex to) const;
  [[nodiscard]] std::optional<Edge> get_edge(Vertex from, Vertex to) const;
  [[nodiscard]] std::vector<Edge> &incident_edges(Vertex v) { return edges[v]; }

  [[nodiscard]] int32_t degree(Vertex v) const { return edges[v].size(); }

  [[nodiscard]] Rational phase(Vertex v) const {
    return vertices[v].value().phase;
  }

  [[nodiscard]] dd::Qubit qubit(Vertex v) const {
    return vertices[v].value().qubit;
  }

  [[nodiscard]] VertexType type(Vertex v) const {
    return vertices[v].value().type;
  }

  [[nodiscard]] std::optional<VertexData> get_vdata(Vertex v) const {
    return vertices[v];
  }

  [[nodiscard]] std::vector<std::pair<Vertex, VertexData &>> get_vertices();
  [[nodiscard]] std::vector<std::pair<Vertex, Vertex>> get_edges();

  [[nodiscard]] const std::vector<Vertex> &get_inputs() const { return inputs; }

  [[nodiscard]] const std::vector<Vertex> &get_outputs() const {
    return outputs;
  }

  [[nodiscard]] bool is_deleted(Vertex v) const {
    return !vertices[v].has_value();
  }

  [[nodiscard]] bool is_boundary_vertex(Vertex v) const {
    return vertices[v].value().type == VertexType::Boundary;
  }

  [[nodiscard]] bool is_input(Vertex v) const;
  [[nodiscard]] bool is_output(Vertex v) const;

  void add_phase(Vertex v, Rational phase) {
    vertices[v].value().phase += phase;
  }

  void set_phase(Vertex v, Rational phase) {
    vertices[v].value().phase = phase;
  }

  void set_type(Vertex v, VertexType type) { vertices[v].value().type = type; }

  void to_graph_like();

  [[nodiscard]] bool is_identity() const;

  [[nodiscard]] ZXDiagram adjoint() const;

  ZXDiagram &invert();

  ZXDiagram &concat(const ZXDiagram &rhs);

private:
  std::vector<std::vector<Edge>> edges;
  std::vector<std::optional<VertexData>> vertices;
  std::vector<Vertex> deleted;
  std::vector<Vertex> inputs;
  std::vector<Vertex> outputs;
  int32_t nvertices = 0;
  int32_t nedges = 0;

  void add_z_spider(dd::Qubit qubit, std::vector<Vertex> &qubit_vertices,
                    Rational phase = Rational(0, 1),
                    EdgeType type = EdgeType::Simple);
  void add_x_spider(dd::Qubit qubit, std::vector<Vertex> &qubit_vertices,
                    Rational phase = Rational(0, 1),
                    EdgeType type = EdgeType::Simple);
  void add_cnot(dd::Qubit ctrl, dd::Qubit target,
                std::vector<Vertex> &qubit_vertices);

  std::vector<Vertex> init_graph(int nqubits);
  void close_graph(std::vector<Vertex> &qubit_vertices);

  void remove_half_edge(Vertex from, Vertex to);

  std::vector<Edge>::iterator get_edge_ptr(Vertex from, Vertex to);
};
} // namespace zx
#endif /* JKQZX_INCLUDE_GRAPH_HPP_ */
