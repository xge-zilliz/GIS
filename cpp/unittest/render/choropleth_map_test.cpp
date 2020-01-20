#include <arrow/render_api.h>
#include "gtest/gtest.h"
#include "render/2d/choropleth_map.h"


using namespace zilliz::render;

TEST(TWOD_TEST, HEATMAP_TEST) {
    ChoroplethMap<uint32_t> choropleth_map;

    // param1: wkt string
    std::string wkt_string1 = "POLYGON (("
                              "-73.98128 40.754771, "
                              "-73.980185 40.754771, "
                              "-73.980185 40.755587, "
                              "-73.98128 40.755587, "
                              "-73.98128 40.754771))";
    arrow::StringBuilder string_builder;
    auto status = string_builder.Append(wkt_string1);

    std::shared_ptr<arrow::StringArray> string_array;
    status = string_builder.Finish(&string_array);

    // param2: color
    std::shared_ptr<arrow::Array> color_array;
    arrow::UInt32Builder color_builder;
    status = color_builder.Append(5);
    status = color_builder.Finish(&color_array);

    get_choropleth_map(string_array, color_array);
}



