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

#ifndef OPHIDIAN_GEOMETRY_GEOMETRY_H
#define OPHIDIAN_GEOMETRY_GEOMETRY_H

#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/multi_polygon.hpp>

#include <boost/geometry/strategies/transform/matrix_transformers.hpp>
#include <boost/geometry/algorithms/correct.hpp>
#include <boost/geometry/algorithms/transform.hpp>
#include <boost/geometry/algorithms/equals.hpp>

#include <boost/geometry/algorithms/intersection.hpp>

#include <boost/geometry/io/wkt/wkt.hpp>

#include <boost/geometry/geometries/geometries.hpp>

#include <boost/geometry/geometry.hpp>

#include <cmath>

namespace ophidian {
namespace geometry {

template <class T>
using point = typename boost::geometry::model::d2::point_xy<T>;
using boost::geometry::model::polygon;
using boost::geometry::model::multi_polygon;

template<class Geometry, class CoordinateType>
void translate(Geometry geometry, point<CoordinateType> point,
		Geometry & result) {
	boost::geometry::strategy::transform::translate_transformer<CoordinateType,
			2, 2> translate(point.x(), point.y());
	boost::geometry::transform(geometry, result, translate);
//	boost::geometry::correct(result);
}


template<class Geometry, class CoordinateType>
void scale(Geometry geometry, point<CoordinateType> point,
                Geometry & result) {
        boost::geometry::strategy::transform::scale_transformer<CoordinateType,
                        2, 2> scaled(point.x(), point.y(), 1.0);
        boost::geometry::transform(geometry, result, scaled);
//	boost::geometry::correct(result);
}

template<class CoordinateType>
CoordinateType manhattan_distance(const point<CoordinateType> & p1, const point<CoordinateType> & p2)
{
	return std::abs(p2.y()-p1.y())+std::abs(p2.x()-p1.x());
}


using boost::geometry::equals;
using boost::geometry::wkt;
using boost::geometry::intersection;
using boost::geometry::subtract_point;
using boost::geometry::append;
using boost::geometry::correct;

}
}

#endif //OPHIDIAN_GEOMETRY_GEOMETRY_H
