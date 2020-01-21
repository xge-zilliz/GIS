#include "vega_weighted_circle2d.h"


namespace zilliz {
namespace render {

VegaWeightedCircle2d::VegaWeightedCircle2d(const std::string &json) {
    Parse(json);
}

std::string
VegaWeightedCircle2d::Build() {
    // TODO: add Build() api to build a vega json string.
    return "";
}


void
VegaWeightedCircle2d::Parse(const std::string &json) {

    rapidjson::Document document;
    document.Parse(json.c_str());

    if (document.Parse(json.c_str()).HasParseError()) {
        printf("json format error\n");
        return;
    }


    if (!JsonLabelCheck(document, "width") ||
        !JsonLabelCheck(document, "height") ||
        !JsonTypeCheck(document["width"], rapidjson::Type::kNumberType) ||
        !JsonTypeCheck(document["height"], rapidjson::Type::kNumberType)) {
        return;
    }
    window_params_.mutable_width() = document["width"].GetInt();
    window_params_.mutable_height() = document["height"].GetInt();


    if (!JsonLabelCheck(document, "marks") ||
        !JsonTypeCheck(document["marks"], rapidjson::Type::kArrayType) ||
        !JsonSizeCheck(document["marks"], "marks", 1) ||
        !JsonLabelCheck(document["marks"][0], "encode") ||
        !JsonLabelCheck(document["marks"][0]["encode"], "enter")) {
        return;
    }
    rapidjson::Value mark_enter;
    mark_enter = document["marks"][0]["encode"]["enter"];

    if (!JsonLabelCheck(mark_enter, "color_range_start") ||
        !JsonLabelCheck(mark_enter, "color_range_start") ||
        !JsonLabelCheck(mark_enter, "color_ruler") ||
        !JsonLabelCheck(mark_enter["color_range_start"], "value") ||
        !JsonLabelCheck(mark_enter["color_range_end"], "value") ||
        !JsonLabelCheck(mark_enter["color_ruler"], "value") ||
        !JsonTypeCheck(mark_enter["color_range_start"]["value"], rapidjson::Type::kStringType) ||
        !JsonTypeCheck(mark_enter["color_range_end"]["value"], rapidjson::Type::kStringType) ||
        !JsonTypeCheck(mark_enter["color_ruler"]["value"], rapidjson::Type::kArrayType) ) {
        return;
    }
//
//    for (int i = 0; i < 2; i++) {
//        if (!JsonTypeCheck(mark_enter["color_range"]["value"][i], rapidjson::Type::kStringType) ||
//            !JsonTypeCheck(mark_enter["color_ruler"]["value"][i], rapidjson::Type::kStringType)) {
//            return;
//        }
//    }

    color_range_ = std::make_pair(ColorParser(mark_enter["color_range_start"]["value"].GetString()).color(),ColorParser(mark_enter["color_range_end"]["value"].GetString()).color());
    color_ruler_ = std::make_pair(mark_enter["color_ruler"]["value"][0].GetInt(),mark_enter["color_ruler"]["value"][1].GetInt());
}


} //namespace render
} //namespace zilliz


