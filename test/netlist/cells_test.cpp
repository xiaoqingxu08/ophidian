#include "../catch.hpp"

#include <entity.h>
#include <cells.h>
#include <nets.h>
#include <pins.h>

TEST_CASE("cells/assign name","[netlist][cells]") {
	openeda::entity::system cells_system;
	openeda::netlist::cells cells(cells_system);
	auto u1 = cells_system.create();
	cells.name(u1, "u1");
	REQUIRE(cells.name(u1) == "u1");
}

TEST_CASE("cells/assign cell_type", "[netlist][cells]") {
	openeda::entity::system cells_system;
	openeda::netlist::cells cells(cells_system);
	openeda::standard_cell::standard_cells std_cells;
	auto NAND2_X2 = std_cells.create("NAND2_X2");
	auto u1 = cells_system.create();
	cells.standard_cell(u1, NAND2_X2);
	REQUIRE(cells.standard_cell(u1) == NAND2_X2);
}

TEST_CASE("cells/insert pin", "[netlist][cells]") {
	openeda::entity::system cells_system;
	openeda::netlist::cells cells(cells_system);
	openeda::entity::system pins_system;
	openeda::netlist::pins pins(pins_system);
	auto u1 = cells_system.create();
	auto u1a = pins_system.create();
	cells.insert_pin(u1, u1a);
	REQUIRE(
			std::find(cells.pins(u1).begin(), cells.pins(u1).end(), u1a)
					!= cells.pins(u1).end());
}

TEST_CASE("cells/assign pins", "[netlist][cells]") {
	openeda::entity::system cells_system;
	openeda::netlist::cells cells(cells_system);
	openeda::entity::system pins_system;
	openeda::netlist::pins pins(pins_system);
	auto u1 = cells_system.create();
	std::vector<openeda::entity::entity> cell_pins { pins_system.create(),
			pins_system.create(), pins_system.create() };
	cells.pins(u1, cell_pins);
	REQUIRE(cell_pins == cells.pins(u1));
}

TEST_CASE("cells/find interconnections of a cell", "[netlist][cells]") {
	openeda::entity::system cells_system;
	openeda::netlist::cells cells(cells_system);
	openeda::entity::system pins_system;
	openeda::netlist::pins pins(pins_system);
	openeda::entity::system nets_system;
	openeda::netlist::nets nets(nets_system);

	auto u1 = cells_system.create();
	auto u1o = pins_system.create();
	auto n1 = nets_system.create();

	nets.name(n1, "n1");

	nets.connect(n1, u1o);
	pins.net(u1o, n1);
	cells.insert_pin(u1, u1o);
	pins.owner(u1o, u1);

	REQUIRE(nets.name(pins.net(cells.pins(u1).front())) == "n1");
}

TEST_CASE("cells/check cells connected to an interconnection", "[netlist][cells]") {
	openeda::entity::system cells_system;
	openeda::netlist::cells cells(cells_system);
	openeda::entity::system pins_system;
	openeda::netlist::pins pins(pins_system);
	openeda::entity::system nets_system;
	openeda::netlist::nets nets(nets_system);

	auto u1 = cells_system.create();
	auto u1o = pins_system.create();
	cells.insert_pin(u1, u1o);
	pins.owner(u1o, u1);

	auto u2 = cells_system.create();
	auto u2a = pins_system.create();
	cells.insert_pin(u2, u2a);
	pins.owner(u2a, u2);

	auto u3 = cells_system.create();
	auto u4 = cells_system.create();

	auto inp1 = nets_system.create();
	auto n1 = nets_system.create();
	auto n2 = nets_system.create();

	pins.net(u1o, n1); nets.connect(n1, u1o);
	pins.net(u2a, n1); nets.connect(n1, u2a);

	auto interconnection_pins = nets.pins(n1);
	std::vector<openeda::entity::entity> pins_owners;
	for( auto p : interconnection_pins )
		pins_owners.push_back(pins.owner(p));

	REQUIRE( pins_owners.size() == 2 );
	REQUIRE( std::find(pins_owners.begin(), pins_owners.end(), u1) != pins_owners.end() );
	REQUIRE( std::find(pins_owners.begin(), pins_owners.end(), u2) != pins_owners.end() );
	REQUIRE( std::find(pins_owners.begin(), pins_owners.end(), u3) == pins_owners.end() );
	REQUIRE( std::find(pins_owners.begin(), pins_owners.end(), u4) == pins_owners.end() );
}
