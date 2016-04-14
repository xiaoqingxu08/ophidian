/*
 *
 * This file is part of Ophidian.
 * Ophidian is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ophidian is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Ophidian.  If not, see <http://www.gnu.org/licenses/>.
 *
*/

#include "canvas.h"

namespace ophidian {
namespace gui {

canvas::canvas()
{
    m_lines.register_property(&m_lines_indices);
}

void canvas::line_destroy(const line &l)
{
    auto entity = l.entity();
    m_lines_vertex_array[m_lines_indices[m_lines.lookup(entity)]] = m_lines_vertex_array[m_lines_vertex_array.getVertexCount()-2];
    m_lines_vertex_array[m_lines_indices[m_lines.lookup(entity)]+1] = m_lines_vertex_array[m_lines_vertex_array.getVertexCount()-1];
    m_lines_vertex_array.resize(m_lines_vertex_array.getVertexCount()-2);
    m_lines.destroy(l.entity());
}

void canvas::wireframe_polygon_destroy(const wireframe_polygon &p)
{
    for(auto l : p)
        line_destroy(l);
}

void canvas::transform(const line &l, const matrix &m)
{
    auto entity = l.entity();
    std::size_t begin = m_lines_indices[m_lines.lookup(entity)];
    for(std::size_t i{begin}; i < begin+2; ++i)
    {
        auto & vertex = m_lines_vertex_array[i];
        // apply
    }
}

line canvas::create_line(const point &u, const point &v)
{
    auto entity = m_lines.create();
    m_lines_indices[m_lines.lookup(entity)] = m_lines_vertex_array.getVertexCount();
    sf::Vertex vertex1(u);
    sf::Vertex vertex2(v);
    m_lines_vertex_array.append(vertex1);
    m_lines_vertex_array.append(vertex2);
    return line(*this, entity);
}

wireframe_polygon canvas::create_wireframe_polygon(const std::vector<point> &points)
{
    wireframe_polygon result;
    for(std::size_t i = 1; i < points.size(); ++i)
    {
        auto u = points[i-1];
        auto v = points[i];
        result.append(create_line(u, v));
    }
    return result;
}

void canvas::draw(sf::RenderTarget &target)
{
    target.draw(&m_lines_vertex_array[0], m_lines_vertex_array.getVertexCount(), sf::PrimitiveType::Lines);
}

line::line() :
    m_canvas(nullptr)
{

}

line::line(canvas &c, entity::entity entity) :
    m_canvas(&c),
    m_entity(entity)
{

}

line::~line()
{

}

void line::transform(const matrix &m)
{
    m_canvas->transform(*this, m);
}

wireframe_polygon::wireframe_polygon()
{

}

void wireframe_polygon::append(const line &l)
{
    m_lines.push_back(l);
}


}
}
