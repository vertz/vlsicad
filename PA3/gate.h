#ifndef _GATE_HH_
#define _GATE_HH_

#include <vector>

#include <boost/shared_ptr.hpp>

class Gate
{
public:
    int gateID;
    int idx;
    double x, y;
    
    std::vector<int> conn_gates;
    std::vector<int> nets;

    Gate(int _id) : gateID(_id), x(0.0), y(0.0) {}
    ~Gate() {}
};

//# Boost Shared Pointer for Gate
typedef boost::shared_ptr<Gate> SPGate;

bool comareTo_horizontal(SPGate lhs, SPGate rhs)
{
    if(lhs->x < rhs->x)
        return true;
    else if(lhs->x == rhs->x)
        return lhs->y < rhs->y;
        
    return  false;
}

bool comareTo_vertical(SPGate lhs, SPGate rhs)
{
    if(lhs->y < rhs->y)
        return true;
    else if(lhs->y == rhs->y)
        return lhs->x < rhs->x;
        
    return  false;
}

#endif // _GATE_HH_
