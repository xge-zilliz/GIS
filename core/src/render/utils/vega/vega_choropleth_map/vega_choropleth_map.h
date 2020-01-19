#pragma once

#include "render/utils/color/color.h"
#include "render/utils/vega/vega.h"


namespace zilliz {
namespace render {


class VegaChoroplethMap : public Vega {
 public:
    struct BoundingBox {
        double longitude_left;
        double latitude_left;
        double longitude_right;
        double latitude_right;
    };

 public:
    VegaChoroplethMap() = default;

    VegaChoroplethMap(const std::string &json);

    std::string
    Build() final;

    const BoundingBox &
    bounding_box() const { return bounding_box_; }

    const std::pair<double, double> &
    ruler() const { return ruler_; }

    const ColorStyle &
    color_style() const { return color_style_; }

    const double &
    opacity() const { return opacity_; }

 private:
    // vega json to vega struct
    void
    Parse(const std::string &json) final;

 private:
    BoundingBox bounding_box_;
    std::pair<double, double> ruler_;
    ColorStyle color_style_;
    double opacity_;
};


} // namespace render
} // namespace zilliz