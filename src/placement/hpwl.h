#ifndef OPHIDIAN_PLACEMENT_HPWL_H
#define OPHIDIAN_PLACEMENT_HPWL_H

namespace ophidian {

namespace entity {
class entity;
}

namespace placement {

class placement;
class hpwl
{
    double m_value;
public:
    hpwl(const placement & place);
    hpwl(const placement & place, const entity::entity & net);
    double value() const {
        return m_value;
    }
};

}
}


#endif // OPHIDIAN_PLACEMENT_HPWL_H