/*
 * Copyright (C) 2019-2020 Zilliz. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <string>
#include <utility>
#include <vector>

#include "render/utils/my_zlib_compress.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb/stb_image_write.h"

namespace zilliz {
namespace render {

std::pair<uint8_t*, int64_t> pointmap(uint32_t* arr_x, uint32_t* arr_y, int64_t num,
                                      const std::string& conf) {
  PointMap point_map(arr_x, arr_y, num);

  VegaCircle2d vega_circle_2d(conf);
  point_map.mutable_point_vega() = vega_circle_2d;

  const auto& render = point_map.Render();
  const auto& ret_size = point_map.output_image_size();
  return std::make_pair(render, ret_size);
}

template <typename T>
std::pair<uint8_t*, int64_t> heatmap(uint32_t* arr_x, uint32_t* arr_y, T* arr_c,
                                     int64_t num_vertices, const std::string& conf) {
  HeatMap<T> heat_map(arr_x, arr_y, arr_c, num_vertices);

  VegaHeatMap vega_heat_map(conf);
  heat_map.mutable_heatmap_vega() = vega_heat_map;

  const auto& render = heat_map.Render();
  const auto& ret_size = heat_map.output_image_size();
  return std::make_pair(render, ret_size);
}

template <typename T>
std::pair<uint8_t*, int64_t> choroplethmap(const std::vector<std::string>& arr_wkt,
                                           T* arr_c, int64_t num_buildings,
                                           const std::string& conf) {
  ChoroplethMap<T> choropleth_map(arr_wkt, arr_c, num_buildings);

  VegaChoroplethMap vega_choropleth_map(conf);
  choropleth_map.mutable_choroplethmap_vega() = vega_choropleth_map;

  const auto& render = choropleth_map.Render();
  const auto& ret_size = choropleth_map.output_image_size();

  return std::make_pair(render, ret_size);
}

std::pair<uint8_t*, int64_t> colorize_pointmap(uint32_t* arr_x, uint32_t* arr_y, int64_t num,
                                      const std::string& conf) {
  PointMap point_map(arr_x, arr_y, num);

  VegaCircle2d vega_circle_2d(conf);
  point_map.mutable_point_vega() = vega_circle_2d;

  const auto& render = point_map.Colorize();
  const auto& ret_size = point_map.buffer_size();
  return std::make_pair(render, ret_size);
}

template <typename T>
std::pair<uint8_t*, int64_t> colorize_heatmap(uint32_t* arr_x, uint32_t* arr_y, T* arr_c,
                                     int64_t num_vertices, const std::string& conf) {
  HeatMap<T> heat_map(arr_x, arr_y, arr_c, num_vertices);

  VegaHeatMap vega_heat_map(conf);
  heat_map.mutable_heatmap_vega() = vega_heat_map;

  const auto& render = heat_map.Colorize();
  const auto& ret_size = heat_map.buffer_size();
  return std::make_pair(render, ret_size);
}

template <typename T>
std::pair<uint8_t*, int64_t> colorize_choroplethmap(const std::vector<std::string>& arr_wkt,
                                           T* arr_c, int64_t num_buildings,
                                           const std::string& conf) {
  ChoroplethMap<T> choropleth_map(arr_wkt, arr_c, num_buildings);

  VegaChoroplethMap vega_choropleth_map(conf);
  choropleth_map.mutable_choroplethmap_vega() = vega_choropleth_map;

  const auto& render = choropleth_map.Colorize();
  const auto& ret_size = choropleth_map.buffer_size();

  return std::make_pair(render, ret_size);
}

std::pair<uint8_t*, int64_t> Render(uint8_t* buffer, int height, int width) {
  // export image to memory
  auto pixels = buffer + (int)(width * 4 * height - 1);
  auto stride_bytes = -(width * 4);

  int output_image_size = 0;
  auto output_image = stbi_write_png_to_mem(pixels, stride_bytes, width, height, 4, &output_image_size);

  auto write_image = true;

  if (write_image) {
    // TODO: add log here
    std::cout << "******************" << output_image_size << "******************"
              << std::endl;
    FILE* f = fopen("/tmp/offscreen.png", "wb");
    if (!f) {
      std::cout << "export png error";
    } else {
      fwrite(output_image, 1, output_image_size, f);
      fclose(f);
    }
  }

  return std::make_pair(output_image, output_image_size);
}

}  // namespace render
}  // namespace zilliz
