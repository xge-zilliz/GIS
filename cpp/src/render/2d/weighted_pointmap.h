#pragma once

#include "render/utils/vega/vega_scatter_plot/vega_weighted_circle2d.h"
#include "render/2d/input.h"
#include "render/2d/general_2d.h"

namespace zilliz {
namespace render {

template<typename T>
class WeightedPointMap : public General2D {
 public:
    WeightedPointMap();

    WeightedPointMap(uint32_t* input_x, uint32_t* input_y, T* count, int64_t num_vertices);

    void
    DataInit() final;

    uint8_t*
    Render() final;

    void
    Shader() final;

    void
    Draw() final;

    void
    InputInit() final;

 public:
    uint32_t*
    mutable_vertices_x() { return vertices_x_; }

    uint32_t*
    mutable_vertices_y() { return vertices_y_; }

    VegaWeightedCircle2d&
    mutable_weighted_point_vega() {return weighted_point_vega_; }

    const size_t
    num_vertices() const { return num_vertices_; }

    void
    set_colors();

 private:
    unsigned int VAO_;
    unsigned int VBO_[3];
    uint32_t* vertices_x_;
    uint32_t* vertices_y_;
    T* count_;
    float *colors_;
    size_t num_vertices_;
    VegaWeightedCircle2d weighted_point_vega_;

};

} //namespace render
} //namespace zilliz
