//
// Created by root on 12/10/19.
//

#ifndef GRAPHICS_ASSIGNMENT_II_HELPERS_H
#define GRAPHICS_ASSIGNMENT_II_HELPERS_H
//
// Created by root on 12/10/19.
//

#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include "vec.h"
#include "matrix.h"
#include <iostream>
#include <iomanip>
#include <glad/glad.h>

typedef GLuint buffer;

struct WavefrontOBJ {


    struct Face {
        std::map<int, std::vector<unsigned int>> indices;
    };

    std::vector<vec3> coordinates;
    std::vector<vec3> texture_coordinates;
    std::vector<vec3> normals;
    std::vector<Face> faces;
};

struct vertex_ {
    vec3 position;
    vec3 uv;
    vec3 normal;
};

struct TriangleMeshData {
    TriangleMeshData(WavefrontOBJ obj);

    std::vector<vertex_> vertices;
    std::vector<int[3]> vertex_indices;
    std::vector<int[3]> neighbor_indices;
    std::vector<int> adjacent_triangle;
};

struct SceneObject {
    vec4 position = {0, 0, 0, 0};

    TriangleMeshData *mesh = nullptr;

    typedef float delta;

    mat4x4 transform(delta x, delta y, delta z);
};

struct GeometryBatch {
    buffer vao;
    buffer vertex_data;
    buffer element_data;
    int element_count;

	GeometryBatch(TriangleMeshData& mesh);
	GeometryBatch(std::vector<vec3> points);

    class handle {
        GeometryBatch *parent;
    public:
        handle(GeometryBatch *parent) : parent(parent) {
            glBindVertexArray(parent->vao);
        }

        void draw() {
            glBindVertexArray(parent->vao);
            glDrawArrays(GL_TRIANGLES, 0, parent->element_count);
        }

        ~handle() { glBindVertexArray(0); }
    };

    handle bind() { return handle(this); }

    ~GeometryBatch() {
        // todo del
        glDeleteVertexArrays(1, &vao);
    }
};

WavefrontOBJ loadOBJ(std::string filename);

void log(std::string tag, std::string msg);

void log_error(std::string tag, std::string msg);

#endif //GRAPHICS_ASSIGNMENT_II_HELPERS_H
