#pragma once

#include <string>
#include "render/utils/vega/vega_scatter_plot/vega_scatter_plot.h"

namespace zilliz {
namespace render {


class VegaWeightedCircle2d : public VegaScatterPlot {
 public:
    VegaWeightedCircle2d() = default;

    VegaWeightedCircle2d(const std::string &json);

    std::string
    Build() final;

    const CircleParams
    circle_params() const { return circle_params_; }

 public:
    const std::pair<Color, Color>&
    color_range() const { return color_range_; }

    const std::pair<double, double>&
    color_ruler() const { return color_ruler_; }

 private:
    // vega json to vega struct
    void
    Parse(const std::string &json) final;

 private:
    std::pair<Color, Color> color_range_;
    std::pair<double, double> color_ruler_;
    VegaScatterPlot::CircleParams circle_params_;
};

} //namespace render
} //namespace zilliz
