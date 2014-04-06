#ifndef _NETLIST_HH_
#define _NETLIST_HH_

#include <algorithm>
#include <iostream> 
#include <valarray>
#include <fstream>
#include <vector>
#include <cmath>
#include <map>

#include "netpart.h"
#include "solver.h"
#include "gate.h"

class NetList
{
private:
    std::vector<std::vector<int> > nets_g; // gates in net
    std::vector<std::vector<int> > nets_p; // pads  in net
    
    std::map<int, SPGate> gates_map;
    std::vector<SPGate> gates;
    
    valarray<int> pinX;
    valarray<int> pinY;
    
    std::vector<SPNetPart> parts;
 
    void init_gates();
    void sort_gates(NetPart &part, bool horizontal);
    
    void propagate_gate(NetPart &part, double curr_x, double curr_y, double &x, double &y); 
    void quadratic_placement_iter(NetPart &part, double x_val, bool gt = false);
    
public:
    NetList() {}
    NetList(std::string fname);
    
    ~NetList() {}
    
    void quadratic_placement(int depth = 1);
};

const double edge_weight = 1.0;

template<typename T>
void print_vector(vector<T>& v) {
  for (size_t i = 0; i < v.size(); ++i) {
    std::cout << v[i] << ",";
  }
  std::cout << std::endl;
}

#endif // _NETLIST_HH_
