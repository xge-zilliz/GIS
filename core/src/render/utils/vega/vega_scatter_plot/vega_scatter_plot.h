#pragma once

#include "render/utils/vega/vega.h"
#include "render/utils/color/color.h"

namespace zilliz {
namespace render {

struct CircleParams {
    int radius;
    Color color;
};

class VegaScatterPlot: public Vega {
 public:
    virtual std::string
    Build() = 0;

 protected:
    // vega json to vega struct
    virtual void
    Parse(const std::string& json) = 0;
};


} // namespace render
} // namespace zilliz