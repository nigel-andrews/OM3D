#ifndef IMGUIRENDERER_H
#define IMGUIRENDERER_H

#include <Material.h>

#include <chrono>

struct ImDrawData;
struct GLFWwindow;

namespace OM3D {

class ImGuiRenderer : NonMovable {
    public:
        ImGuiRenderer(GLFWwindow* window);

        void start();
        void finish();

        enum class DebugMode {
            NONE = 0,
            ALBEDO = 1,
            NORMALS = 2,
            DEPTH = 3,
        } debug_mode = DebugMode::NONE;

    private:
        void render(const ImDrawData* draw_data);
        float update_delta_time();

        GLFWwindow* _window = nullptr;

        Material _material;
        std::unique_ptr<Texture> _font;
        std::chrono::time_point<std::chrono::high_resolution_clock> _last;
};

}

#endif // IMGUIRENDERER_H
