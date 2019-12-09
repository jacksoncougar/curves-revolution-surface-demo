
#include <string>
#include <cassert>
#include <iterator>
#include "helpers.h"
#include "transform.h"


WavefrontOBJ loadOBJ(const std::string filename) {
    std::ifstream file;
    file.open(filename);

    assert(file.is_open());

    WavefrontOBJ obj;
    std::string line;
    std::string type;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        ss >> type;

        if (type == "#") {
            std::cout << "[Wavefront] Comment: '" + line + "'\n";
        } else if (type == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            obj.coordinates.emplace_back(x, y, z);
        } else if (type == "vn") {
            float nx, ny, nz;
            ss >> nx >> ny >> nz;
            obj.normals.emplace_back(nx, ny, nz);
        } else if (type == "vt") {
            float s(0), t(0), u(0);
            ss >> s >> t >> u;
            obj.texture_coordinates.emplace_back(s, t, u);
        } else if (type == "f") {
            obj.faces.emplace_back(*std::istream_iterator<WavefrontOBJ::Face>(ss));
        } else { std::cerr << "[Wavefront] Unknown line while parsing OBJ file: '" + line + "'\n"; }

    }

    return obj;
}

std::istream &operator>>(std::istream &in, WavefrontOBJ::Face &face) {
    int index = 0;
    int value;
    while (in >> value) {
        face.indices[index].emplace_back(value);
        while (in.peek() == '/' && in.get()) { index++; }; // read one or more.
        if (in.peek() == ' ') index = 0; // finished reading face.
    }
    return in;
}


void log(std::string tag, std::string msg) {
    std::cout << std::setw(12) << std::left << "[" + tag + "] "
              << std::setw(0) << msg << std::endl;
}

void log_error(std::string tag, std::string msg) {
    std::cerr << std::setw(12) << std::left << "[" + tag + "] "
              << std::setw(0) << msg << std::endl;
}

GeometryBatch::GeometryBatch(TriangleMeshData &mesh) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vertex_data);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_data);
    glBufferData(GL_ARRAY_BUFFER,
                 mesh.vertices.size() * sizeof(vertex_),
                 mesh.vertices.data(),
                 GL_STATIC_DRAW);

    glBindVertexArray(vao);

    element_count = mesh.vertices.size();

    glBindBuffer(GL_ARRAY_BUFFER, vertex_data);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_), (void *) 0);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_data);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_), (void *) sizeof(vec4));

    glBindBuffer(GL_ARRAY_BUFFER, vertex_data);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(vertex_), (void *) (2 * sizeof(vec4)));

    glBindVertexArray(0);
}

GeometryBatch::GeometryBatch(std::vector<vec3> points)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vertex_data);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_data);
	glBufferData(GL_ARRAY_BUFFER,
		points.size() * sizeof(vec3),
		points.data(),
		GL_STATIC_DRAW);

	element_count = points.size();

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_data);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
	

	glBindVertexArray(0);
}

mat4x4 SceneObject::transform(SceneObject::delta x, SceneObject::delta y, SceneObject::delta z) {
    position = position + vec4(x, y, z, 1);
    return Transform::translate(position.x, position.y, position.z);
}

TriangleMeshData::TriangleMeshData(WavefrontOBJ obj) {
    for (auto face : obj.faces) {
        vec3 n1{}, n2{}, n3{};
        vec3 uv1{}, uv2{}, uv3{};

        auto a = obj.coordinates[(face.indices[0][0] - 1)];
        auto b = obj.coordinates[(face.indices[0][1] - 1)];
        auto c = obj.coordinates[(face.indices[0][2] - 1)];

        if (face.indices.count(1) > 0) {
            uv1 = obj.texture_coordinates[face.indices[1][0] - 1];
            uv2 = obj.texture_coordinates[face.indices[1][1] - 1];
            uv3 = obj.texture_coordinates[face.indices[1][2] - 1];
        } else {
            uv1 = {0.5, 1, 0};
            uv2 = {0, 0, 0};
            uv3 = {1, 0, 0};
        }
        if (face.indices.count(2) > 0) {
            n1 = obj.normals[face.indices[2][0] - 1];
            n2 = obj.normals[face.indices[2][1] - 1];
            n3 = obj.normals[face.indices[2][2] - 1];
        } else {
            auto ba = b - a;
            auto ca = c - a;
            n1 = n2 = n3 = ba.cross(ca).norm();
        }

        vertices.push_back({a, uv1, n1});
        vertices.push_back({b, uv2, n2});
        vertices.push_back({c, uv3, n3});
    }
}
