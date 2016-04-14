#include <iostream>
#include "../timing-driven_placement/timingdriven_placement.h"
#include "../../src/geometry/geometry.h"
#include <fstream>



#include <random>

struct command_line_args {
    std::string dot_v;
    std::string dot_def;
    std::string dot_lef;
    std::string dot_lib_late;
    std::string dot_lib_early;
    double clock;
    command_line_args(){}
    command_line_args(char **argv){
        dot_v = argv[1];
        dot_def = argv[2];
        dot_lef = argv[3];
        dot_lib_late = argv[4];
        dot_lib_early = argv[5];
        clock = std::stod(argv[6]);
    }
};


struct vertex {
    double x,y,z;

};
std::ostream& operator<<(std::ostream& out, const vertex & v) {
    return out << "v " << v.x << " " << v.z  << " " << v.y << " 1.0 0.0 0.0";
}
struct face {
    std::vector<std::size_t> vertices;
    face() : vertices(4){}
};

std::ostream& operator<<(std::ostream& out, const face & f) {
    return out << "f " << f.vertices[0] << " " <<  f.vertices[1] << " " <<  f.vertices[2] << " " <<  f.vertices[3];
}

struct rectangle {
    std::vector<vertex> vertices;
    rectangle() : vertices(4){}
};
std::ostream& operator<<(std::ostream& out, const rectangle & f) {
    for(auto vertex : f.vertices)
        out << vertex << std::endl;
    return out;
}

struct cube {
    std::vector<vertex> vertices;
    std::vector<face> faces;

    cube(const rectangle & rect, double height) :
        vertices(rect.vertices),
        faces(6)
    {
        for(auto v : rect.vertices)
            vertices.push_back({v.x, v.y, v.z+height});
        faces[0].vertices = {1,2,3,4};
        faces[1].vertices = {5,6,7,8};
        faces[2].vertices = {1,5,6,2};
        faces[3].vertices = {4,8,7,3};
        faces[4].vertices = {1,5,8,4};
        faces[5].vertices = {2,6,7,3};
    }

    void print(std::ostream & out, std::size_t curr) {

        for(auto v : vertices)
            out << v << std::endl;

        for(auto f : faces)
        {
            out << "f ";
            out << 8*curr+f.vertices[0] << " ";
            out << 8*curr+f.vertices[1] << " ";
            out << 8*curr+f.vertices[2] << " ";
            out << 8*curr+f.vertices[3] << std::endl;
        }

    }
};


std::ostream& operator<<(std::ostream& out, const cube & c) {
    for(auto vertex : c.vertices)
        out << vertex << std::endl;

    for(auto face : c.faces)
        out << face << std::endl;
    return out;
}



struct material {
    std::string name;
    double ns{94.117647};
    double ka[3]{1.000000, 1.000000, 1.000000};
    double kd[3];
    double ks[3]{0.789157, 0.789157, 0.789157};
    double ke[3]{0.000000, 0.000000, 0.000000};
    double ni{1.000000};
    double d{1.000000};
    double illum{2};
};

// 255 0 0
// 255 62 0
// 255 125 0
// 255 187 0
// 255 255 0
// 187 255 0
// 125 255 0
// 62 255 0
// 0 255 0


int main(int argc, char **argv) {

    using namespace ophidian;

    // reads command line arguments
    command_line_args args;

    std::string ckt = argv[4];


    args.dot_v = "benchmarks/"+ckt+"/"+ckt+".v";
    args.dot_def = "benchmarks/"+ckt+"/"+ckt+".def";
    args.dot_lef = "benchmarks/"+ckt+"/"+ckt+".lef";
    args.dot_lib_late = "benchmarks/"+ckt+"/"+ckt+"_Late.lib";
    args.dot_lib_early = "benchmarks/"+ckt+"/"+ckt+"_Early.lib";
    args.clock = 5000;


    double origx = std::stod(argv[1]);
    double origy = std::stod(argv[2]);
    double size = std::stod(argv[3]);


    // initializes the TDP module
    timingdriven_placement::timingdriven_placement tdp(args.dot_v, args.dot_def, args.dot_lef, args.dot_lib_late, args.dot_lib_early, args.clock);

    int num_colors = 10;

    // gets all cells and geometries
    auto cells_geometries = tdp.cells_geometries();
    auto & cells = cells_geometries.cells;
    auto & geometries = cells_geometries.geometries;

    // iterates over each cell and prints its name and geometry in WKT format
    //    for(std::size_t i{0}; i < cells.size(); ++i)
    //    {
    //        std::cout << tdp.cell_name(cells[i]);
    //        std::cout << "\t" << geometry::wkt(geometries[i]) << std::endl;
    //    }


    std::ofstream out("city.obj", std::ofstream::out);
    //    std::ostream & out = std::cout;
    std::size_t current{1};

    std::vector<rectangle> rects;
    std::vector<entity::entity> entities;
    std::vector<bool> macroblock;


    std::uniform_int_distribution<int> dist(1,5);
    std::default_random_engine engine;



    const geometry::point<double> ORIGIN(origx,origy);
    const geometry::point<double> SIZE(size*300000, size*300000);
    const double SCALING_FACTOR{1.0/3420.0};

    geometry::polygon< geometry::point<double> > window;
    geometry::append(window, ORIGIN);
    geometry::append(window, geometry::point<double>(ORIGIN.x(),ORIGIN.y()+SIZE.y()));
    geometry::append(window, geometry::point<double>(ORIGIN.x()+SIZE.x(),ORIGIN.y()+SIZE.y()));
    geometry::append(window, geometry::point<double>(ORIGIN.x()+SIZE.x(),ORIGIN.y()));
    geometry::append(window, ORIGIN);
    geometry::correct(window);


    std::cout << "creating rects" << std::endl;

    std::size_t curr_index = 0;
    std::size_t macroblock_count = 0;
    for(auto const & cell_multipolygon : geometries)
    {
        bool is_macroblock = (cell_multipolygon.size()!=1);
        if(is_macroblock)
            ++macroblock_count;
        for(auto const & cell_polygon : cell_multipolygon)
        {
            geometry::multi_polygon<geometry::polygon<geometry::point<double> > > result;
            geometry::intersection(window, cell_polygon, result);
            if(result.empty()) continue;
            rectangle new_rect;
            for(std::size_t k{0}; k < 4; ++k)
            {
                geometry::point<double> point = cell_polygon.outer()[k];
                geometry::subtract_point(point, ORIGIN);
                new_rect.vertices[k] = {point.x()*SCALING_FACTOR, point.y()*SCALING_FACTOR, 1.0};
            }
            rects.push_back(new_rect);
            entities.push_back(cells[curr_index]);
            macroblock.push_back(is_macroblock);
        }
        ++curr_index;
    }


    std::cout << "extruding rects" << std::endl;
    std::vector<cube> cubes;
    cubes.reserve(rects.size());

    std::size_t i = 0;
    for(auto r : rects)
    {
        int height = tdp.cell_pins(entities[i]).size();
        if(macroblock[i])
            height = log(height);
        cubes.emplace_back(r, static_cast<double>(height));
        ++i;
    }


    std::cout << "writing cubes to file" << std::endl;

    i = 0;
    for(auto cube : cubes)
        cube.print(out, i++);


    std::cout << macroblock_count << " macroblocks" << std::endl;
    return 0;
}
