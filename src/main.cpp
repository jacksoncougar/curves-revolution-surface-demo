//
// Created by root on 25/11/19.
//

#include "camera.h"
#include "curve.h"
#include "helpers.h"
#include "image.h"
#include "shading.h"
#include "vec.h"

#include <glad/glad.h>
// glad must come before glfw3
#include <GLFW/glfw3.h>

#include "subdivision.h"
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

struct Scene
{

  struct Viewport
  {
    float x = 0;
    float y = 0;
    float width = 0;
    float height = 0;
    vec4 color{0.2, 0.2, 0.2, 0.2};

    void clear()
    {
      glViewport(x, y, width, height);
      glScissor(x, y, width, height);

      auto &[r, g, b, a] = color;
      glClearColor(r, g, b, a);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    bool contains(float ux, float uy)
    {
      return x < ux && ux < x + width && y < uy && uy < y + height;
    }

    void bind() const
    {
      glViewport(x, y, width, height);
      glScissor(x, y, width, height);
    }
  };

  struct Camera
  {
    vec3 position = {0, 0, 0};
    vec3 rotation = {0, 0, 0};
    bool enabled = false;
  } camera;
  ;

  std::vector<Viewport> viewport{
      Viewport{0, 0, 320, 480},
      Viewport{320, 0, 320, 480}};

  std::unique_ptr<Program> program;
  std::unique_ptr<Program> program2;

  std::unique_ptr<control_curve> ccurve;

  std::unique_ptr<mesh> revolution_mesh;

  void update_mesh(std::vector<vec3> points)
  {
    const auto subdivided_curve = ::curve(subdivide(points, 4));
    revolution_mesh.reset(new ::mesh(enmesh(subdivided_curve)));
    subdivided_curve.destroy();
  }
};

static float gtime = 0;

void draw_control_curve(GLFWwindow *window, control_curve &curve)
{
  {
    auto _ = curve.bind();
    glEnable(GL_DEPTH_TEST);
    glPointSize(5);
    glDrawArrays(GL_POINTS, 0, curve.points.size());
    glDrawArrays(GL_LINE_STRIP, 0, curve.points.size());
  }
  {
    const auto subdivided_curve = ::curve(subdivide(curve.points, 4));
    auto _ = subdivided_curve.bind();
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_LINE_STRIP, 0, subdivided_curve.points.size());
    subdivided_curve.destroy();
  }
}

void save_model(const Scene &scene)
{
  // save control curve
  std::ofstream file("model.data", std::ios::binary);
  auto size = scene.ccurve->points.size();
  file.write((char *)&size, sizeof(size_t));
  file.write(
      (char *)scene.ccurve->points.data(),
      sizeof(vec3) * scene.ccurve->points.size());
}

void load_model(Scene &scene)
{
  std::ifstream file("model.data", std::ios::binary);
  if (file.is_open())
  {
    size_t count;
    std::vector<vec3> points;
    file.read((char *)&count, sizeof(size_t));
    points.resize(count);
    file.read((char *)points.data(), sizeof(vec3) * count);

    scene.ccurve.reset(new control_curve(
        points,
        std::bind(&Scene::update_mesh, &scene, std::placeholders::_1)));
  }
}

void draw(GLFWwindow *window, Scene *scene)
{
  gtime += 0.008;
  if (!scene->program)
    return;

  for (auto viewport : scene->viewport)
    viewport.clear();

  {
    auto look_at =
        Camera::create_lookat_matrix({0, 0, 10.0001f}, {0, 1, 0}, {0, 0, 0});
    auto perspective = Camera::create_orthographic_matrix(
        {scene->viewport[0].width, scene->viewport[0].height});

    scene->program2->bind();

    mat4x4 world = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    glUniformMatrix4fv(
        glGetUniformLocation(scene->program2->id(), "view"),
        1,
        false,
        look_at.data());
    glUniformMatrix4fv(
        glGetUniformLocation(scene->program2->id(), "projection"),
        1,
        false,
        perspective.data());
    glUniformMatrix4fv(
        glGetUniformLocation(scene->program2->id(), "world"),
        1,
        false,
        world.data());

    scene->viewport[0].bind();
    draw_control_curve(window, *scene->ccurve);
  }
  {
    scene->program->bind();

    auto look_at =
        Camera::create_lookat_matrix({0, 0, 2.5}, {0, 1, 0}, {0, 0, 0});
    auto perspective = Camera::create_orthographic_matrix(
        {scene->viewport[0].width,
         scene->viewport[0].height,
         30._deg,
         0.001f,
         10.0f});
    auto world = Transform::rotate_about_y(scene->camera.rotation.y) *
                 Transform::rotate_about_x(scene->camera.rotation.x);

    glUniformMatrix4fv(
        glGetUniformLocation(scene->program->id(), "view"),
        1,
        false,
        look_at.data());
    glUniformMatrix4fv(
        glGetUniformLocation(scene->program->id(), "projection"),
        1,
        false,
        perspective.data());
    glUniformMatrix4fv(
        glGetUniformLocation(scene->program->id(), "world"),
        1,
        false,
        world.data());

    scene->viewport[1].bind();

    if (scene->revolution_mesh)
    {
      auto handle = scene->revolution_mesh->bind();

      glEnable(GL_DEPTH_TEST);
      glDrawArrays(GL_TRIANGLES, 0, scene->revolution_mesh->data.size());
    }
  }
  glfwSwapBuffers(window);
}

inline Scene *get_window_scene(GLFWwindow *window)
{
  const auto user_pointer = glfwGetWindowUserPointer(window);
  return static_cast<Scene *>(user_pointer);
}

void window_size_callback(GLFWwindow *window, int width, int height)
{
  auto scene = get_window_scene(window);
  if (!scene)
    return;

  scene->viewport[0] = {0, 0, width / 2.0f, static_cast<float>(height)};
  scene->viewport[1] =
      {width / 2.0f, 0, width / 2.0f, static_cast<float>(height)};
}

void window_refresh_callback(GLFWwindow *window)
{
  const auto scene = get_window_scene(window);
  if (!scene)
    return;

  int width, height;
  glfwGetWindowSize(window, &width, &height);

  draw(window, scene);
}

ray project(float x, float y, float width, float height)
{
  auto look_at = Camera::create_lookat_matrix({0, 0, 3}, {0, 1, 0}, {0, 0, 0});
  auto perspective = Camera::create_orthographic_matrix({width, height});

  auto left = -0.5 * 1.0 / perspective.m00 * 2.0;   // are you happy now?
  auto bottom = -0.5 * 1.0 / perspective.m11 * 2.0; //

  const auto u = look_at.rows(0);
  const auto v = look_at.rows(1);
  const auto w = look_at.rows(2);
  const auto origin = look_at.columns(3);

  x = left + 2.0f * std::abs(left) * x / width;
  y = bottom + 2.0f * std::abs(bottom) * y / height;

  const auto point = (origin + x * u + y * v).swizzle<0, 1, 2>();
  const ray r = {point, w.swizzle<0, 1, 2>()};

  return r;
}

void cursor_position_callback(GLFWwindow *window, double x, double y)
{
  static double mx(x / 2), my(y);
  auto scene = get_window_scene(window);

  y = scene->viewport[0].height - y;

  auto dx = x - mx;
  auto dy = y - my;

  auto [s, _] =
      project(0, 0, scene->viewport[0].width, scene->viewport[0].height);
  auto [f, __] =
      project(dx, dy, scene->viewport[0].width, scene->viewport[0].height);

  auto ddx = f.x - s.x;
  auto ddy = f.y - s.y;

  if (scene->viewport[0].contains(x, y))
    for (size_t i = 0; i < scene->ccurve->points.size(); ++i)
    {
      if (scene->ccurve->is_selected[i])
      {
        scene->ccurve->points[i].x += ddx;
        scene->ccurve->points[i].y += ddy;
        scene->ccurve->handles[i].surface.origin.x += ddx;
        scene->ccurve->handles[i].surface.origin.y += ddy;
        scene->ccurve->update_batch();
      }
    }

  if (scene->camera.enabled)
  {
    scene->camera.rotation.x += ddy;
    scene->camera.rotation.y += ddx;
  }

  mx = x;
  my = y;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
  auto scene = get_window_scene(window);

  double x, y;
  glfwGetCursorPos(window, &x, &y);

  if (scene->viewport[0].contains(x, y))
  {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
      const auto r = project(
          x,
          scene->viewport[0].height - y,
          scene->viewport[0].width,
          scene->viewport[0].height);

      for (auto &&traceable : scene->ccurve->handles)
      {
        if (traceable.test(r))
        {
          scene->ccurve->select(traceable);
          break;
        }
      }
    }
  }
  else if (scene->viewport[1].contains(x, y))
  {
    static float mx(x), my(y);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
      mx = x, my = y;
      scene->camera.enabled = true;
    }
  }

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
  {
    scene->ccurve->clear_selections();
    scene->camera.enabled = false;
  }
}

void key_callback(
    GLFWwindow *window,
    int key,
    int scancode,
    int action,
    int mods)
{
  auto scene = get_window_scene(window);
  if (key == GLFW_KEY_S && action == GLFW_PRESS)
  {
    scene->ccurve->split_selected();
  }
  if (key == GLFW_KEY_D && action == GLFW_PRESS)
  {
    scene->ccurve->delete_selected();
  }
  if (key == GLFW_KEY_E && action == GLFW_PRESS)
  {
    scene->ccurve->duplicate_selected();
  }

  if (key == GLFW_KEY_S && action == GLFW_PRESS && mods & GLFW_MOD_CONTROL)
  {
    save_model(*scene);
  }

  if (key == GLFW_KEY_L && action == GLFW_PRESS && mods & GLFW_MOD_CONTROL)
  {
    load_model(*scene);
  }
}

GLFWwindow *initialize_glfw(int width, int height)
{
  if (!glfwInit())
  {
    log_error("GLFW", "Failed to init glfw");
    exit(-1);
  }

  glfwWindowHint(GLFW_DEPTH_BITS, GLFW_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  GLFWwindow *window = glfwCreateWindow(
      width,
      height,
      "CPSC 453 Curves Assignment",
      nullptr,
      nullptr);

  if (!window)
  {
    log_error("GLFW", "Failed to create window");
    glfwTerminate();
    exit(-1);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glfwSetWindowSizeCallback(window, window_size_callback);
  glfwSetWindowRefreshCallback(window, window_refresh_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);
  glfwSetKeyCallback(window, key_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    log_error("GLAD", "Failed to initialize glad");
    exit(-2);
  }

  log("OpenGL",
      "version: '" + std::string((char *)glGetString(GL_VERSION)) + "'");

  return window;
}

int main(int argc, char *argv[])
{

  Scene scene;

  const auto window =
      initialize_glfw(static_cast<int>(640), static_cast<int>(480));

  scene.program.reset(new Program{
      Shader<GL_VERTEX_SHADER>("shaders/phong.vert.glsl"),
      Shader<GL_FRAGMENT_SHADER>("shaders/phong.frag.glsl")});

  scene.program2.reset(new Program{
      Shader<GL_VERTEX_SHADER>("shaders/flat.vert.glsl"),
      Shader<GL_FRAGMENT_SHADER>("shaders/flat.frag.glsl")});

  glfwSetWindowUserPointer(window, &scene);

  scene.ccurve.reset(new control_curve(
      std::vector<vec3>{
          {0.1, 0.5, 0},
          {0.5, 0, 0},
          {0.1, -0.5, 0},
          {0.2, 0, 0}},
      std::bind(&Scene::update_mesh, &scene, std::placeholders::_1)));

  scene.program->bind();
  glUniform1i(glGetUniformLocation(scene.program->id(), "talbedo"), 3);
  glUniform1i(glGetUniformLocation(scene.program->id(), "tnormal"), 0);
  glUniform1i(glGetUniformLocation(scene.program->id(), "tao"), 1);
  glUniform1i(glGetUniformLocation(scene.program->id(), "troughness"), 2);

  std::vector<GLuint> textures;
  textures.reserve(4);

  for (size_t i = 1; i < argc; i++)
  {
    auto filename = argv[i];
    auto image = raster::read_image_from_file(filename);

    glActiveTexture(GL_TEXTURE0 + (i - 1));

    textures.push_back(0);

    glGenTextures(1, &textures.back());
    glBindTexture(GL_TEXTURE_2D, textures.back());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto channels = GL_RGBA;
    if (image.channels() == 3)
      channels = GL_RGB;

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        channels,
        image.width(),
        image.height(),
        0,
        channels,
        GL_UNSIGNED_BYTE,
        image.data());

    glGenerateMipmap(GL_TEXTURE_2D);
  }
  if (textures.size() > 0)
  {
    glUniform1i(glGetUniformLocation(scene.program->id(), "textured"), true);
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_SCISSOR_TEST);

  glDepthFunc(GL_LESS);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glPointSize(10);

  glfwSetWindowSize(window, 640, 480);

  while (!glfwWindowShouldClose(window))
  {

    draw(window, &scene);
    glfwPollEvents();
  }
}