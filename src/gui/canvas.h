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

#ifndef OPHIDIAN_GUI_CANVAS_H
#define OPHIDIAN_GUI_CANVAS_H

#include <SFML/Graphics.hpp>
#include "../entity/entity.h"
#include "../entity/vector_property.h"

namespace ophidian {
namespace gui {


using point = sf::Vector2f;
class canvas;

class matrix {

};


class line {
    canvas * m_canvas;
    entity::entity m_entity;

public:
    line();
    line(canvas & c, entity::entity entity);
    virtual ~line();

    const entity::entity & entity() const {
        return m_entity;
    }

    void transform(const matrix & m);


};


class wireframe_polygon {
    std::vector<line> m_lines;
public:
    wireframe_polygon();
    void append(const line & l);

    std::vector<line>::const_iterator begin() const{
        return m_lines.begin();
    }
    std::vector<line>::const_iterator end() const {
        return m_lines.end();
    }

};

class canvas
{
    entity::system m_lines;
    entity::vector_property< std::size_t > m_lines_indices;

    sf::VertexArray m_lines_vertex_array;

public:
    canvas();

    void line_destroy(const line& l);
    void wireframe_polygon_destroy(const wireframe_polygon & p);

    void transform(const line& l, const matrix & m);
    line create_line(const point & u, const point & v);
    wireframe_polygon create_wireframe_polygon(const std::vector<point> & points);


    void draw(sf::RenderTarget & target);


};

}
}

#endif // OPHIDIAN_GUI_CANVAS_H
