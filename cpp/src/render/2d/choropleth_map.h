#pragma once

#include "render/utils/vega/vega_choropleth_map/vega_choropleth_map.h"
#include "render/2d/general_2d.h"


namespace zilliz {
namespace render {

template<typename T>
class ChoroplethMap : public General2D {
 public:
    ChoroplethMap();

    ChoroplethMap(std::vector<std::string> choropleth_wkt, T *count, int64_t num_vertices);

    std::shared_ptr<uint8_t>
    Render() final;

    void
    Draw() final;

    void
    DataInit() final {};

    void
    InputInit() final {};

    VegaChoroplethMap&
    mutable_choroplethmap_vega() {return choropleth_vega_; }

 private:
    void
    Transform();

    void
    SetColor();

 private:
    std::vector<std::string> choropleth_wkt_;
    T *count_;
    int64_t num_buildings_;
    VegaChoroplethMap choropleth_vega_;

    std::vector<std::vector<int>> buildings_x_;
    std::vector<std::vector<int>> buildings_y_;
    std::vector<float> colors_;
};

} //namespace render
} //namespace zilliz

