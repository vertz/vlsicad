#ifndef _GATE_HH_
#define _GATE_HH_

#include <vector>

#include <boost/shared_ptr.hpp>

class Gate
{
public:
    int gateID;
    unsigned idx;
    double x, y;
    
    std::vector<int> gates_weight;
    std::vector<int> conn_gates;
    std::vector<int> conn_pads;
    std::vector<int> nets;
    
    Gate(int _id) : gateID(_id), x(0.0), y(0.0) {}
    ~Gate() {}
    
    void init_gate();
};

//# Boost Shared Pointer for Gate
typedef boost::shared_ptr<Gate> SPGate;

bool comareTo_horizontal(SPGate lhs, SPGate rhs);
bool comareTo_vertical(SPGate lhs, SPGate rhs);

#endif // _GATE_HH_
