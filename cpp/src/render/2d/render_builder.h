#pragma once
#include "render/2d/pointmap.h"
#include "render/2d/heatmap.h"
#include "render/2d/choropleth_map.h"

namespace zilliz {
namespace render {

std::pair<std::shared_ptr<uint8_t>, int64_t>
pointmap(uint32_t* arr_x, uint32_t arr_y, int64_t num_vertices);

template<typename T>
std::pair<std::shared_ptr<uint8_t>, int64_t>
heatmap(uint32_t arr_x, uint32_t arr_y, T* arr_c, int64_t num_vertices);

template<typename T>
std::pair<std::shared_ptr<uint8_t>, int64_t>
choroplethmap(std::string arr_wkt, T* arr_c, int64_t num_buildings);

} //namespace render
} //namespace zilliz
#include "render/2d/render_builder_impl.h"