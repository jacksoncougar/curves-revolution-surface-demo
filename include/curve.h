//
// Created by root on 25/11/19.
//

#ifndef GFX_A_IV_CURVE_H
#define GFX_A_IV_CURVE_H


#include "vec.h"
#include <vector>
#include "ray.h"
#include "hit.h"
#include "sphere.h"

#include <glad/glad.h>
#include "subdivision.h"
#include <map>
#include <tuple>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <functional>

struct face
{
	vec3 normal;
};

namespace std {
	template <> struct hash<vec3>
	{
		size_t operator()(const vec3& vec) const noexcept
		{
			return std::hash<float>{}(vec.x) ^ (std::hash<float>{}(vec.x) << 1)
				^ (std::hash<float>{}(vec.x) << 2);
		}
	};
	template <> struct hash<vertex>
	{
		size_t operator()(const vertex& c) const noexcept
		{
			auto& [position, normal, uv] = c;
			return std::hash<vec3>{}(position) ^ (std::hash<vec3>{}(normal) << 1)
				^ (std::hash<vec3>{}(uv) << 2);
		}
	};
}

struct winged_edge
{
	std::map<const vertex*, std::vector<const face*>> lookup;
	std::unordered_set<vertex> vertices;
	std::vector<face> faces;

	const std::vector<const face*>& faces_of(const vertex& vertex)
	{
		const auto result = vertices.find(vertex);
		if (result != vertices.end())
		{
			return lookup[&*result];
		}
		return {};
	}

	winged_edge(const std::vector<tri<vertex>>& triangles)
	{
		faces.reserve(triangles.size());
		vertices.reserve(triangles.size());

		for (const auto& [a, b, c] : triangles)
		{
			const auto normal = normal_of(a.position, b.position, c.position);

			auto [va, ignored1] = vertices.emplace(a);
			auto [vb, ignored2] = vertices.emplace(b);
			auto [vc, ignored3] = vertices.emplace(c);

			faces.push_back(face{ normal });

			auto [ait, a_new] = lookup.emplace(&*va, std::vector<const face*>{ &faces.back() });
			auto [bit, b_new] = lookup.emplace(&*vb, std::vector<const face*>{ &faces.back() });
			auto [cit, c_new] = lookup.emplace(&*vc, std::vector<const face*>{ &faces.back() });

			if (!a_new) ait->second.emplace_back(&faces.back());
			if (!b_new) bit->second.emplace_back(&faces.back());
			if (!c_new) cit->second.emplace_back(&faces.back());
		}
	}
};

struct batch
{
	typedef GLuint buffer;
	buffer vao;
	buffer array;

	struct handle
	{
		handle(const buffer vao)
		{
			glBindVertexArray(vao);
		}
		~handle() { glBindVertexArray(0); }
	};

	
	handle bind() const
	{
		return handle{ vao };
	}

	void destroy() const
	{
		glDeleteBuffers(1, &array);
		glDeleteVertexArrays(1, &vao);
	}
};

struct mesh : public batch
{
	struct vertex_format
	{
		vec3 position;
		vec3 normal;
		vec3 uv;

		vertex_format(vec3 position, vec3 normal, vec3 uv) : position(position), normal(normal), uv(uv){}
	};
	
	std::vector<vertex_format> data;

	
	explicit mesh(const std::vector<tri<vertex>>& faces) : batch()
	{
		auto wing = winged_edge(faces);

		data.reserve(faces.size() * 3);

		for (auto& [a, b, c] : faces)
		{
			{
				auto adjacent = wing.faces_of(a);
				auto average_normal = std::accumulate(adjacent.begin(), adjacent.end(), vec3{ 0,0,0 },
					[](vec3 normal, const face* f) {return normal + f->normal; }).norm();
				
				data.emplace_back(a.position, average_normal, a.uv);
			}
			{
				auto adjacent = wing.faces_of(b);
				auto average_normal = std::accumulate(adjacent.begin(), adjacent.end(), vec3{ 0,0,0 },
					[](vec3 normal, const face* f) {return normal + f->normal; }).norm();
				
				data.emplace_back(b.position, average_normal, b.uv);
			}
			{
				auto adjacent = wing.faces_of(c);
				auto average_normal = std::accumulate(adjacent.begin(), adjacent.end(), vec3{ 0,0,0 },
					[](vec3 normal, const face* f) {return normal + f->normal; }).norm();
				
				data.emplace_back(c.position, average_normal, c.uv);
			}
		}

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &array);

		glBindBuffer(GL_ARRAY_BUFFER, array);
		glBufferData(GL_ARRAY_BUFFER,
			data.size() * sizeof(vertex_format),
			data.data(),
			GL_DYNAMIC_DRAW);


		glBindBuffer(GL_ARRAY_BUFFER, array);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_format), (void*)0);


		glBindBuffer(GL_ARRAY_BUFFER, array);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_format), (void*)sizeof(vec3));

		glBindBuffer(GL_ARRAY_BUFFER, array);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_format), (void*)(2 * sizeof(vec3)));

		glBindVertexArray(0);
	}
};


struct curve : public batch {
	std::vector<vec3> points;

	curve(const std::vector<vec3>& points) : batch(), points(points)
	{
		std::vector<vec3> data;
		data.reserve(points.size() * 2);

		for (auto& point : points)
		{
			data.emplace_back(point);
			data.emplace_back(0.7,0,0.7);
		}
		
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		
		glGenBuffers(1, &array);

		glBindBuffer(GL_ARRAY_BUFFER, array);
		glBufferData(GL_ARRAY_BUFFER,
			data.size() * sizeof(vec3),
			data.data(),
			GL_DYNAMIC_DRAW);


		glBindBuffer(GL_ARRAY_BUFFER, array);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) * 2, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, array);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) * 2, (void*)sizeof(vec3));

		glBindVertexArray(0);
	}
};

struct control_curve : public batch
{
	std::vector<vec3> points;
	std::vector<traceable<sphere>> handles;
	std::vector<bool> is_selected;

	const float radius = 0.05f;

	std::function<void(const std::vector<vec3>&)> dirty_handler;
	
	
	control_curve() = default;

	explicit control_curve(const std::vector<vec3>& points, std::function<void(const std::vector<vec3>&)> handler = nullptr)
		: batch(), points(points), is_selected(points.size()), dirty_handler(handler)
	{
		for(auto point : points)
		{
			handles.emplace_back(sphere{point, radius});
		}

		std::vector<vec3> data;
		data.reserve(points.size() * 2);

		for (size_t i = 0; i < points.size(); i++)
		{
			data.emplace_back(points[i]);
			data.emplace_back((is_selected[i] ? vec3{1.0f, 1.0f, 0.0f} : vec3{ .1f, .1f, .1f}));
		}

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &array);

		glBindBuffer(GL_ARRAY_BUFFER, array);
		glBufferData(GL_ARRAY_BUFFER,
			data.size() * sizeof(vec3),
			data.data(),
			GL_STATIC_DRAW);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, array);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) * 2, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, array);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) * 2, (void*)sizeof(vec3));

		glBindVertexArray(0);
		on_dirty();
	}

	void on_dirty() const
	{
		if (dirty_handler) dirty_handler(points);
	}

	void delete_selected()
	{
		if (is_selected.size() <= 3) return;
		
		for (size_t i = 0; i < is_selected.size(); i++)
		{
			if (is_selected[i])
			{
				points.erase(points.begin() + i);
				is_selected.erase(is_selected.begin() + i);
				handles.erase(handles.begin() + i);

				update_batch();
				on_dirty();
				break;
			}
		}
	}
	
	void split_selected()
	{
		for (size_t i = 0; i < is_selected.size(); i++)
		{
			if(is_selected[i])
			{
				if (i + 1 < points.size())
				{
					const auto a = points[i];
					const auto b = points[i + 1];
					const auto ab = 0.5 * (a + b);
					points.insert(points.begin() + i + 1, ab);
					is_selected.insert(is_selected.begin() + i + 1, false);
					handles.insert(handles.begin() + i + 1, traceable<sphere>( ab, radius ));
				}
				else if(i > 0)
				{
					const auto a = points[i];
					const auto b = points[i - 1];
					const auto ab = 0.5 * (a + b);
					points.insert(points.begin() + i, ab);
					is_selected.insert(is_selected.begin() + i, false);
					handles.insert(handles.begin() + i, traceable<sphere>( ab, radius ));
				}
				replace_batch();
				on_dirty();
				break;
			}
		}
	}

	void duplicate_selected()
	{
		for (size_t i = 0; i < is_selected.size(); i++)
		{
			if (is_selected[i])
			{
				const auto a = points[i];
				auto index = i;
				points.insert(points.begin() + index, a);
				is_selected.insert(is_selected.begin() + index, false);
				handles.insert(handles.begin() + index, traceable<sphere>(a, radius));
				replace_batch();
				break;
			}
		}
	}

	void clear_selections()
	{
		auto changed = false;
		for (auto&& value : is_selected)
		{
			changed |= value;
			value = false;
		}

		if (changed) {
			update_batch();
			on_dirty();
		}
	}

	void update_batch()
	{
		std::vector<vec3> data;
		data.reserve(points.size() * 2);

		for (size_t i = 0; i < points.size(); i++)
		{
			data.emplace_back(points[i]);
			data.emplace_back((is_selected[i] ? vec3{ 1.f, 1.f, 0.f } : vec3{ .1f, .1f, .1f }));
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, array);
		glBufferSubData(GL_ARRAY_BUFFER,
		                0,
		                data.size() * sizeof(vec3),
		                data.data());

		glBindVertexArray(vao);
	}

	void replace_batch()
	{
		std::vector<vec3> data;
		data.reserve(points.size() * 2);

		for (size_t i = 0; i < points.size(); i++)
		{
			data.emplace_back(points[i]);
			data.emplace_back((is_selected[i] ? vec3{ 1.f, 1.f, 0.f } : vec3{ .1f, .1f, .1f }));
		}

		glBindBuffer(GL_ARRAY_BUFFER, array);
		glBufferData(GL_ARRAY_BUFFER,
			data.size() * sizeof(vec3),
			data.data(), GL_DYNAMIC_DRAW);

		glBindVertexArray(vao);
	}

	void select(const traceable<sphere> &handle)
	{
		const auto it = std::find(handles.begin(), handles.end(), handle);
		if(it != handles.end())
		{
			const auto index = std::distance(handles.begin(), it);
			is_selected[index] = true;
		}


		update_batch();
	}
};


#endif //GFX_A_IV_CURVE_H
