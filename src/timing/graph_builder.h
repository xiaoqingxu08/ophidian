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

#ifndef SRC_TIMING_GRAPH_BUILDER_H_
#define SRC_TIMING_GRAPH_BUILDER_H_

#include "../netlist/netlist.h"
#include "graph.h"
#include "library.h"
#include "design_constraints.h"

namespace ophidian {
namespace timing {

class graph_builder {
public:
	static void build(const netlist::netlist & netlist, library & lib, const timing::design_constraints & dc, graph& graph);
};

} /* namespace timing */
} /* namespace openeda */

#endif /* SRC_TIMING_GRAPH_BUILDER_H_ */
