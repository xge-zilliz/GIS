#include <iostream>
#include "render_api.h"
#include "render/2d/render_builder.h"

namespace zilliz {
namespace render {

std::shared_ptr<arrow::Array>
get_pointmap(std::shared_ptr<arrow::Array> arr_x, std::shared_ptr<arrow::Array> arr_y) {
    auto x_length = arr_x->length();
    auto y_length = arr_y->length();
    auto x_type = arr_x->type_id();
    auto y_type = arr_y->type_id();
    assert(x_length == y_length);
    assert(x_type == arrow::Type::UINT32);
    assert(y_type == arrow::Type::UINT32);
    int64_t num_vertices = x_length;

    //array{ArrayData{vector<Buffer{uint8_t*}>}}
    auto input_x = (uint32_t *) arr_x->data()->GetValues<uint8_t>(1);
    auto input_y = (uint32_t *) arr_y->data()->GetValues<uint8_t>(1);
//    auto input_x = std::shared_ptr<uint32_t>(x_data);
//    auto input_y = std::shared_ptr<uint32_t>(y_data);

    auto output = pointmap(input_x, input_y, num_vertices);

    auto output_length = output.second;
    auto output_data = output.first;
    auto bit_map = (uint8_t*)malloc(output_length);
    memset(bit_map, output_length, 0xff);
    auto buffer0 = std::make_shared<arrow::Buffer>(bit_map, output_length);
    auto buffer1 = std::make_shared<arrow::Buffer>(output_data.get(), output_length);
    auto buffers = std::vector<std::shared_ptr<arrow::Buffer>>();
    buffers.emplace_back(buffer0);
    buffers.emplace_back(buffer1);

    auto data_type = arrow::uint8();
    auto array_data = arrow::ArrayData::Make(data_type, output_length, buffers);
    auto array = arrow::MakeArray(array_data);
    return array;
}

std::shared_ptr<arrow::Array>
get_heatmap(std::shared_ptr<arrow::Array> arr_x, std::shared_ptr<arrow::Array> arr_y, std::shared_ptr<arrow::Array> arr_c) {
    auto x_length = arr_x->length();
    auto y_length = arr_y->length();
    auto c_length = arr_c->length();
    auto x_type = arr_x->type_id();
    auto y_type = arr_y->type_id();
    auto c_type = arr_c->type_id();
    assert(x_length == y_length);
    assert(x_length == c_length);
    assert(x_type == arrow::Type::UINT32);
    assert(y_type == arrow::Type::UINT32);
    int64_t num_vertices = x_length;

    //array{ArrayData{vector<Buffer{uint8_t*}>}}
    auto input_x = (uint32_t *) arr_x->data()->GetValues<uint8_t>(1);
    auto input_y = (uint32_t *) arr_y->data()->GetValues<uint8_t>(1);
//    auto input_x = std::shared_ptr<uint32_t>(x_data);
//    auto input_y = std::shared_ptr<uint32_t>(y_data);

    std::pair<std::shared_ptr<uint8_t >,int64_t> output;
    switch(c_type) {
        case arrow::Type::FLOAT : {
            auto input_c_float = (float *) arr_c->data()->GetValues<uint8_t>(1);
//            auto input_c_float = std::shared_ptr<float>(c_data_float);
            output = heatmap<float>(input_x, input_y, input_c_float, num_vertices);
            break;
        }
        case arrow::Type::DOUBLE : {
            auto input_c_double = (double *) arr_c->data()->GetValues<uint8_t>(1);
//            auto input_c_double = std::shared_ptr<double>(c_data_double);
            output = heatmap<double>(input_x, input_y, input_c_double, num_vertices);
            break;
        }
        case arrow::Type::UINT32 : {
            auto input_c_uint32 = (uint32_t *) arr_c->data()->GetValues<uint8_t>(1);
//            auto input_c_uint32 = std::shared_ptr<uint32_t>(c_data_uint32);
            output = heatmap<uint32_t >(input_x, input_y, input_c_uint32, num_vertices);
            break;
        }
        default:
            std::cout << "type error!";
    }

    auto output_length = output.second;
    auto output_data = output.first;
    auto bit_map = (uint8_t*)malloc(output_length);
    memset(bit_map, output_length, 0xff);
    auto buffer0 = std::make_shared<arrow::Buffer>(bit_map, output_length);
    auto buffer1 = std::make_shared<arrow::Buffer>(output_data.get(), output_length);
    auto buffers = std::vector<std::shared_ptr<arrow::Buffer>>();
    buffers.emplace_back(buffer0);
    buffers.emplace_back(buffer1);

    auto data_type = arrow::uint8();
    auto array_data = arrow::ArrayData::Make(data_type, output_length, buffers);
    auto array = arrow::MakeArray(array_data);
    return array;
}

std::shared_ptr<arrow::Array>
get_choropleth_map(std::shared_ptr<arrow::Array> arr_wkt, std::shared_ptr<arrow::Array> arr_color) {

    auto arr_wkt_length = arr_wkt->length();
    auto arr_color_length = arr_color->length();
    auto wkt_type = arr_wkt->type_id();
    auto color_type = arr_color->type_id();
    assert(arr_wkt_length == arr_color_length);
    assert(wkt_type == arrow::Type::STRING);
    assert(color_type == arrow::Type::UINT32);
    int64_t num_buildings = arr_wkt_length;

    auto string_array = std::static_pointer_cast<arrow::StringArray>(arr_wkt);
    std::vector<std::string> input_wkt(arr_wkt_length);
    for (int i = 0; i < arr_wkt_length; i++) {
        input_wkt[i] = string_array->GetString(i);
    }
    auto input_color = (uint32_t *) arr_color->data()->GetValues<uint8_t>(1);

    auto output = choroplethmap(input_wkt, input_color, num_buildings);

    auto output_length = output.second;
    auto output_data = output.first;
    auto bit_map = (uint8_t*)malloc(output_length);
    memset(bit_map, output_length, 0xff);
    auto buffer0 = std::make_shared<arrow::Buffer>(bit_map, output_length);
    auto buffer1 = std::make_shared<arrow::Buffer>(output_data.get(), output_length);
    auto buffers = std::vector<std::shared_ptr<arrow::Buffer>>();
    buffers.emplace_back(buffer0);
    buffers.emplace_back(buffer1);

    auto data_type = arrow::uint8();
    auto array_data = arrow::ArrayData::Make(data_type, output_length, buffers);
    auto array = arrow::MakeArray(array_data);
    return array;
}

} //namespace render
} //namespace zilliz
