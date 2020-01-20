#pragma once

#include <vector>

#include "color.h"
#include "render/utils/vega/vega_scatter_plot/vega_scatter_plot.h"


namespace zilliz {
namespace render {

class ColorGradient
{
 private:
    struct ColorPoint
    {
        float r,g,b;
        float val;
        ColorPoint(float red, float green, float blue, float value)
            : r(red), g(green), b(blue), val(value) {}
    };
    std::vector<ColorPoint> color;

 public:
    static CircleParams
    GetCircleParams(ColorStyle color_style, double ratio);

    ColorGradient()  {  createDefaultHeatMapGradient();  }

    void createDefaultHeatMapGradient();

    void getColorAtValue(const float value, float &red, float &green, float &blue);
};

} // namespace render
} // namespace zilliz


