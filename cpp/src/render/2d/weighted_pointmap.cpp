#include <map>
#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <iostream>
#include "render/utils/color/color_gradient.h"
#include "render/2d/weighted_pointmap.h"


namespace zilliz {
namespace render {

template
class WeightedPointMap<int8_t>;

template
class WeightedPointMap<int16_t>;

template
class WeightedPointMap<int32_t>;

template
class WeightedPointMap<int64_t>;

template
class WeightedPointMap<uint8_t>;

template
class WeightedPointMap<uint16_t>;

template
class WeightedPointMap<uint32_t>;

template
class WeightedPointMap<uint64_t>;

template
class WeightedPointMap<float>;

template
class WeightedPointMap<double>;

template<typename T>
WeightedPointMap<T>::WeightedPointMap()
    : vertices_x_(nullptr), vertices_y_(nullptr), num_vertices_(0) {
}

template<typename T>
WeightedPointMap<T>::WeightedPointMap(uint32_t *input_x, uint32_t *input_y, T *count, int64_t num_vertices)
    : vertices_x_(input_x), vertices_y_(input_y), count_(count), num_vertices_(num_vertices) {
}

template<typename T>
void
WeightedPointMap<T>::InputInit() {
    array_vector_ = input().array_vector;
    VegaWeightedCircle2d vega_weighted_circle_2d(input().vega);
    weighted_point_vega_ = vega_weighted_circle_2d;
}

template<typename T>
void WeightedPointMap<T>::set_colors() {
    auto color_range = weighted_point_vega_.color_range();
    ColorGradient color_gradient;
    color_gradient.createWeightGradient(color_range.first, color_range.second);

    auto color_ruler = weighted_point_vega_.color_ruler();
    int64_t c_offset = 0;
    for (auto j = 0; j < num_vertices_; j++) {
        auto vertice_count = count_[j] >= color_ruler.first ? count_[j] : color_ruler.first;
        vertice_count = count_[j] <= color_ruler.second ? vertice_count : color_ruler.second;
        auto ratio = (vertice_count - color_ruler.first) / (color_ruler.second - color_ruler.first);
        float color_r, color_g, color_b;
        color_gradient.getColorAtValue(ratio, color_r, color_g, color_b);
        colors_[c_offset++] = color_r;
        colors_[c_offset++] = color_g;
        colors_[c_offset++] = color_b;
        colors_[c_offset++] = ratio;
    }
}

template<typename T>
void
WeightedPointMap<T>::DataInit() {
    WindowParams window_params = weighted_point_vega_.window_params();
    int64_t width = window_params.width();
    int64_t height = window_params.height();
    int64_t window_size = width * height;

    colors_ = (float *) malloc(window_size * 4 * sizeof(float));
    set_colors();
}

template<typename T>
void
WeightedPointMap<T>::Draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);

#ifdef USE_GPU
    glDrawArrays(GL_POINTS, 0, num_vertices_);
    glFlush();

    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(2, VBO_);
#else
    glOrtho(0, window()->window_params().width(), 0, window()->window_params().height(), -1, 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    int offset = 0;
    std::vector<int32_t> vertices(num_vertices_ * 2);
    for (auto i = 0; i < num_vertices_; i++) {
        vertices[offset++] = vertices_x_[i];
        vertices[offset++] = vertices_y_[i];
    }
    glColorPointer(4, GL_FLOAT, 0, colors_);
    glVertexPointer(2, GL_INT, 0, &vertices[0]);

    glDrawArrays(GL_POINTS, 0, num_vertices_);
    glFlush();
#endif
}

template<typename T>
void
WeightedPointMap<T>::Shader() {
    const char *vertexShaderSource = \
        "#version 430 core\n"
        "layout (location = 0) in uint posX;\n"
        "layout (location = 1) in uint posY;\n"
        "layout (location = 2) in vec4 point_color;\n"
        "layout (location = 3) uniform vec2 screen_info;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   float tmp_x = posX;\n"
        "   float tmp_y = posY;\n"
        "   gl_Position = vec4(((tmp_x * 2) / screen_info.x) - 1, ((tmp_y * 2) / screen_info.y) - 1, 0, 1);\n"
        "   color=point_color;\n"
        "}";

    const char *fragmentShaderSource = \
        "#version 430 core\n"
        "in vec4 color;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = color;\n"
        "}";

    int success;
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::cout << "vertex shader compile failed.";
    }
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::cout << "fragment shader compile failed.";
    }
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        std::cout << "shader program link failed.";
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(3, VBO_);

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_[0]);
    glBufferData(GL_ARRAY_BUFFER, num_vertices_ * 1 * sizeof(uint32_t), vertices_x_, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_[1]);
    glBufferData(GL_ARRAY_BUFFER, num_vertices_ * 1 * sizeof(uint32_t), vertices_y_, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_[2]);
    glBufferData(GL_ARRAY_BUFFER, num_vertices_ * 4 * sizeof(float), colors_, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_[0]);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_TRUE, 1 * sizeof(uint32_t), (void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_[1]);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_TRUE, 1 * sizeof(uint32_t), (void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_[2]);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 1);
    glBindVertexArray(1);

    glUseProgram(shaderProgram);
    glUniform2f(3, window()->window_params().width(), window()->window_params().height());
    glBindVertexArray(VAO_);
}


template<typename T>
uint8_t *
WeightedPointMap<T>::Render() {
//    InputInit();
    WindowsInit(weighted_point_vega_.window_params());
    DataInit();
#ifdef USE_GPU
    Shader();
#endif
    Draw();
    Finalize();
    return Output();
}

} //namespace render
} //namespace zilliz

