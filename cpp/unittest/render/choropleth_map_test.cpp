#include <arrow/render_api.h>
#include "gtest/gtest.h"
#include "render/2d/choropleth_map.h"


using namespace zilliz::render;

TEST(TWOD_TEST, HEATMAP_TEST) {
    ChoroplethMap<uint32_t> choropleth_map;

    // param1: wkt string
    std::string wkt_string = "POLYGON ((35 10, 45 45, 15 40, 10 20, 35 10),\n"
                             "(20 30, 35 35, 30 20, 20 30))";

    auto bit_map = new uint8_t{0xff};

    auto data_type = arrow::utf8();

    auto buff_data1 = wkt_string.c_str();

    auto buffer0 = std::make_shared<arrow::Buffer>(bit_map, 1 * sizeof(uint8_t));
    auto buffer1 = std::make_shared<arrow::Buffer>((uint8_t *) buff_data1, wkt_string.length());
    std::vector<std::shared_ptr<arrow::Buffer>> buffers1;
    buffers1.emplace_back(buffer0);
    buffers1.emplace_back(buffer1);
    auto array_data1 = arrow::ArrayData::Make(data_type, wkt_string.length(), buffers1);
    auto array1 = arrow::MakeArray(array_data1);

    // param2: color
    auto bit_map2 = new uint8_t{0xff};

    auto buff_data2 = (uint32_t *) malloc(1 * sizeof(uint32_t));

    auto buffer20 = std::make_shared<arrow::Buffer>(bit_map2, 1 * sizeof(uint8_t));
    auto buffer21 = std::make_shared<arrow::Buffer>((uint8_t *) buff_data2, 1 * sizeof(uint32_t));
    std::vector<std::shared_ptr<arrow::Buffer>> buffers2;
    buffers2.emplace_back(buffer20);
    buffers2.emplace_back(buffer21);
    auto array_data2 = arrow::ArrayData::Make(arrow::uint32(), 1 * sizeof(uint32_t), buffers2);
    auto array2 = arrow::MakeArray(array_data2);
    get_choropleth_map(array1, array2);
}



