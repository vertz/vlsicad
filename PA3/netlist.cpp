#include "netlist.h"

NetList::NetList(std::string fname)
{
    std::ifstream f(fname.c_str());
    std::string line;
    
    if(f.fail()){
        std::cerr << "couldn't open the file (\"" << fname << "\")" << std::endl;
        return;
    }
    
    int ngates, nnets, nnc;
    int gateID, idx, n=0;
    SPGate so;
    
    f >> ngates >> nnets;
    nets_g.resize(nnets);
    nets_p.resize(nnets);
    
    while(ngates-- > 0)
    {
        f >> gateID;
        gateID--; // i want them to start from 0 and not from 1
        
        so = SPGate(new Gate(gateID));
        so->idx = n++;
        
        gates_map[gateID] = so;
        gates.push_back(so);
        
        f >> nnc;
        while(nnc-- > 0)
        {
            f >> idx;
            --idx; // i want them to start from 0 and not from 1
            
            nets_g[idx].push_back(gateID);
            so->nets.push_back(idx);
        }
    }
    
    int npads, padID, x, y;
    
    f >> npads;
    pinX.resize(npads);
    pinY.resize(npads);
    
    while(npads-- > 0)
    {
        f >> padID >> idx >> x >> y;
        
        padID--;
        idx--;
        
        pinX[padID] = x;
        pinY[padID] = y;
        
        nets_p[idx].push_back(padID);
    }
    
    f.close();
    init_gates();
}

void NetList::init_gates()
{
    std::vector<int>::iterator nets_it;
    std::vector<int>::iterator it;
    std::vector<int> gate_lst;
    int gateID;

    SPGate so;
    for(int idx=0; idx < gates.size(); ++idx)
    {
        so = gates[idx];
        gateID = so->gateID;
        
        for (nets_it = so->nets.begin() ; nets_it != so->nets.end(); ++nets_it)
        {
            gate_lst = nets_g[*nets_it];
            for (it = gate_lst.begin() ; it != gate_lst.end(); ++it)
            {
                if(*it == gateID)
                    continue;
                    
                so->conn_gates.push_back(*it);   
            }
        }
        
        it = std::unique(so->conn_gates.begin(), so->conn_gates.end());
        so->conn_gates.resize(std::distance(so->conn_gates.begin(), it));
    }
}

void NetList::sort_gates()
{
    std::sort(gates.begin(), gates.end(), comareTo_horizontal);
    
    SPGate so;
    for(int idx=0; idx < gates.size(); ++idx)
    {
        so = gates[idx];
        so->idx = idx;
    }
}

void NetList::quadratic_placement_iter(int s_idx, int e_idx, double x_val, bool gt)
{
    std::vector<int> R, C;
    std::vector<double> V;
    
    size_t sz = e_idx - s_idx;
    int nnz = 0;
    
    valarray<double> bx(sz);
    valarray<double> by(sz);
    
    std::vector<int>::iterator nets_it;
    std::vector<int>::iterator it;
    std::vector<int> gate_lst, pad_lst;
    
    SPGate so, so_neighbor;
    int gateID;
    int n_idx;
    double a_weight;
    double pads_x;
    double pads_y;
    
    std::vector<SPGate>::iterator it_pins_unique;
    std::vector<SPGate>::iterator it_pins;
    std::vector<SPGate> psuedo_pins;
    
    for(int idx = s_idx; idx < e_idx; ++idx)
    {
        so = gates[idx];
        gateID = so->gateID;
        
        a_weight = 0.0;
        pads_x   = 0.0;
        pads_y   = 0.0;
        
        psuedo_pins.clear();
        
        for(it = so->conn_gates.begin() ; it != so->conn_gates.end(); ++it)
        { 
            so_neighbor = gates_map[*it];
            n_idx = so_neighbor->idx;
            if(n_idx < s_idx || n_idx >= e_idx)
            {
                psuedo_pins.push_back(so_neighbor);
                continue;
            }    
                   
            R.push_back(idx - s_idx);
            C.push_back(n_idx - s_idx);
            V.push_back(-edge_weight);
            ++nnz;
                
            a_weight += edge_weight;
        }
        
        for (nets_it = so->nets.begin() ; nets_it != so->nets.end(); ++nets_it)
        {
            pad_lst = nets_p[*nets_it];
            for (it = pad_lst.begin() ; it != pad_lst.end(); ++it)
            {
                a_weight += edge_weight;
                
                if((gt && pinX[*it] > x_val) || (!gt && pinX[*it] < x_val))
                    pads_x += pinX[*it];
                else
                    pads_x += x_val;   
                    
                pads_y += pinY[*it];
            }
        }
        
        it_pins_unique = std::unique (psuedo_pins.begin(), psuedo_pins.end());
        for (it_pins = psuedo_pins.begin() ; it_pins != it_pins_unique; ++it_pins)
        {
            a_weight += edge_weight;
                
            pads_x += x_val;
            pads_y += (*it_pins)->y;
        }
        
        if(a_weight != 0)
        {
            R.push_back(idx - s_idx);
            C.push_back(idx - s_idx);
            V.push_back(a_weight);
            ++nnz;
            
            bx[idx - s_idx] = pads_x; 
            by[idx - s_idx] = pads_y;
        }
    }
    
    coo_matrix A;
    
    A.n = sz;
    A.nnz = nnz;
    A.row.resize(A.nnz);
    A.col.resize(A.nnz);
    A.dat.resize(A.nnz);
    
    A.row = valarray<int>(R.data(), A.nnz);
    A.col = valarray<int>(C.data(), A.nnz);
    A.dat = valarray<double>(V.data(), A.nnz);
    
    valarray<double> x(1.0, A.n);
    valarray<double> y(1.0, A.n);
    
    A.solve(bx, x);
    A.solve(by, y);
    
    for(int idx=s_idx; idx < e_idx; ++idx)
    {
        so = gates[idx];
        
        so->x = x[idx - s_idx];
        so->y = y[idx - s_idx];
    }
    
    std::cout << "fin" << std::endl;
}

void NetList::quadratic_placement(int depth)
{
    int sz = gates.size();
    
    quadratic_placement_iter(0, sz, 100);
    sort_gates();
    
    int parts = 2;
    int elems = sz / 2;
    
    quadratic_placement_iter(0 , elems, 50.0, false);
    quadratic_placement_iter(elems, sz, 50.0, true );
    
    std::ofstream g("test.out");
    if (g.is_open())
    {
        SPGate so;
        for(int idx=0; idx < sz; ++idx)
        {
            so = gates_map[idx];
            g << (idx + 1) << " " << so->x << " " << so->y << std::endl;
        }
        
        g.close();
    }
}

int main()
{
    NetList nl("test.txt");
    nl.quadratic_placement();

    return 0;
}
