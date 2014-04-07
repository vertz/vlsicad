#include "gate.h"

void Gate::init_gate()
{
    std::vector<int>::iterator unique_end;
    std::vector<int>::iterator it_unique;
    std::vector<int>::iterator it;
    
    int weight;
    
    unique_end = std::unique(conn_gates.begin(), conn_gates.end());
    std::sort(conn_gates.begin(), unique_end);
    
    for(it_unique = conn_gates.begin() ; it_unique != unique_end; ++it_unique)
    {
        weight = 1;
        for(it = unique_end ; it != conn_gates.end(); ++it)
        {
            if(*it_unique == *it)
                ++weight;
        }
        
        gates_weight.push_back(weight);
    }
    
    conn_gates.resize(std::distance(conn_gates.begin(), unique_end));
}

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
