#pragma once

namespace zilliz {
namespace render {

std::pair<uint8_t*, int64_t>
pointmap(uint32_t* arr_x, uint32_t* arr_y, int64_t num) {
    PointMap point_map(arr_x, arr_y, num);
    std::string vega = "{\n"
                       "  \"width\": 300,\n"
                       "  \"height\": 200,\n"
                       "  \"description\": \"circle_2d\",\n"
                       "  \"data\": [\n"
                       "    {\n"
                       "      \"name\": \"data\",\n"
                       "      \"url\": \"data/data.csv\"\n"
                       "    }\n"
                       "  ],\n"
                       "  \"scales\": [\n"
                       "    {\n"
                       "      \"name\": \"x\",\n"
                       "      \"type\": \"linear\",\n"
                       "      \"domain\": {\"data\": \"data\", \"field\": \"c0\"}\n"
                       "    },\n"
                       "    {\n"
                       "      \"name\": \"y\",\n"
                       "      \"type\": \"linear\",\n"
                       "      \"domain\": {\"data\": \"data\", \"field\": \"c1\"}\n"
                       "    }\n"
                       "  ],\n"
                       "  \"marks\": [\n"
                       "    {\n"
                       "      \"encode\": {\n"
                       "        \"enter\": {\n"
                       "          \"shape\": {\"value\": \"circle\"},\n"
                       "          \"stroke\": {\"value\": \"#ff0000\"},\n"
                       "          \"strokeWidth\": {\"value\": 30},\n"
                       "          \"opacity\": {\"value\": 0.5}\n"
                       "        }\n"
                       "      }\n"
                       "    }\n"
                       "  ]\n"
                       "}";
    VegaCircle2d vega_circle_2d(vega);
    point_map.mutable_point_vega() = vega_circle_2d;

    const auto &render_output = point_map.Render();
    const auto &output_image_size = point_map.output_image_size();
//    uint8_t* temp = nullptr;
//    int temp1 = 3;

    return std::make_pair(render_output, output_image_size);
//    return std::make_pair(temp, temp1);

}

template<typename T>
std::pair<uint8_t*, int64_t>
heatmap(uint32_t* arr_x,
        uint32_t* arr_y,
        T* arr_c,
        int64_t num_vertices) {
    HeatMap<T> heat_map(arr_x, arr_y, arr_c, num_vertices);
    std::string vega = "{\n"
                       "  \"width\": 300,\n"
                       "  \"height\": 200,\n"
                       "  \"description\": \"circle_2d\",\n"
                       "  \"data\": [\n"
                       "    {\n"
                       "      \"name\": \"data\",\n"
                       "      \"url\": \"data/data.csv\"\n"
                       "    }\n"
                       "  ],\n"
                       "  \"scales\": [\n"
                       "    {\n"
                       "      \"name\": \"x\",\n"
                       "      \"type\": \"linear\",\n"
                       "      \"domain\": {\"data\": \"data\", \"field\": \"c0\"}\n"
                       "    },\n"
                       "    {\n"
                       "      \"name\": \"y\",\n"
                       "      \"type\": \"linear\",\n"
                       "      \"domain\": {\"data\": \"data\", \"field\": \"c1\"}\n"
                       "    }\n"
                       "  ],\n"
                       "  \"marks\": [\n"
                       "    {\n"
                       "      \"encode\": {\n"
                       "        \"enter\": {\n"
                       "          \"map_scale\": {\"value\": 10}\n"
                       "        }\n"
                       "      }\n"
                       "    }\n"
                       "  ]\n"
                       "}";
    VegaHeatMap vega_heat_map(vega);
    heat_map.mutable_heatmap_vega() = vega_heat_map;

    const auto &render_output = heat_map.Render();
    const auto &output_image_size = heat_map.output_image_size();

    return std::make_pair(render_output, output_image_size);
}

template<typename T>
std::pair<uint8_t*, int64_t>
choroplethmap(std::vector<std::string> arr_wkt, T* arr_c, int64_t num_buildings) {
    ChoroplethMap<T> choropleth_map(arr_wkt, arr_c, num_buildings);
    std::string vega = "{\n"
                       "  \"width\": 1900,\n"
                       "  \"height\": 1410,\n"
                       "  \"description\": \"choropleth_map\",\n"
                       "  \"data\": [\n"
                       "    {\n"
                       "      \"name\": \"data\",\n"
                       "      \"url\": \"data/data.csv\"\n"
                       "    }\n"
                       "  ],\n"
                       "  \"scales\": [\n"
                       "    {\n"
                       "      \"name\": \"building\",\n"
                       "      \"type\": \"linear\",\n"
                       "      \"domain\": {\"data\": \"data\", \"field\": \"c0\"}\n"
                       "    }\n"
                       "  ],\n"
                       "  \"marks\": [\n"
                       "    {\n"
                       "      \"encode\": {\n"
                       "        \"enter\": {\n"
                       "          \"bounding_box\": {\"value\": [-73.984092,40.753893,-73.977588,40.756342]},\n"
                       "          \"color_style\": {\"value\": \"blue_to_red\"},\n"
                       "          \"ruler\": {\"value\": [2.5, 5]},\n"
                       "          \"opacity\": {\"value\": 1.0}\n"
                       "        }\n"
                       "      }\n"
                       "    }\n"
                       "  ]\n"
                       "}";
    VegaChoroplethMap vega_choropleth_map(vega);
    choropleth_map.mutable_choroplethmap_vega() = vega_choropleth_map;

    const auto &render_output = choropleth_map.Render();
    const auto &output_image_size = choropleth_map.output_image_size();

    return std::make_pair(render_output, output_image_size);
}

}
}
