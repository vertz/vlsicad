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
    SPNetPart sp = SPNetPart(new NetPart());
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
        
        // init netpart
        if(sp->x_max < x)
            sp->x_max = x;
        else if(sp->x_min <0 || sp->x_min > x) 
            sp->x_min = x;  
            
        if(sp->y_max < y)
            sp->y_max = y;
        else if(sp->y_min <0 || sp->y_min > y) 
            sp->y_min = y;  
    }
    
    sp->s_idx  = 0;
    sp->e_idx  = gates.size();
    parts.push_back(sp);
    
    f.close();
    init_gates();
}

void NetList::init_gates()
{
    std::vector<int>::iterator nets_it;
    std::vector<int>::iterator it;
    std::vector<int> _lst;
    int gateID;

    SPGate so;
    for(int idx=0; idx < gates.size(); ++idx)
    {
        so = gates[idx];
        gateID = so->gateID;
        
        for (nets_it = so->nets.begin() ; nets_it != so->nets.end(); ++nets_it)
        {
            _lst = nets_g[*nets_it];
            for (it = _lst.begin() ; it != _lst.end(); ++it)
            {
                if(*it == gateID)
                    continue;
                    
                so->conn_gates.push_back(*it);   
            }
            
            _lst = nets_p[*nets_it];
            for (it = _lst.begin() ; it != _lst.end(); ++it)
            {
                so->conn_pads.push_back(*it);
            }
        }
        
        it = std::unique(so->conn_gates.begin(), so->conn_gates.end());
        so->conn_gates.resize(std::distance(so->conn_gates.begin(), it));
    }
}

void NetList::sort_gates(NetPart &part, bool horizontal)
{
    if(horizontal)
    {
        std::sort(gates.begin() + part.s_idx, gates.begin() + part.e_idx, comareTo_horizontal);
    }
    else
    {
        std::sort(gates.begin() + part.s_idx, gates.begin() + part.e_idx, comareTo_vertical);
    }
    
    
    SPGate so;
    for(int idx=part.s_idx; idx < part.e_idx; ++idx)
    {
        so = gates[idx];
        so->idx = idx;
    }
}

void NetList::propagate_gate(NetPart &part, double curr_x, double curr_y, double &x, double &y)
{
    if(curr_x < part.x_min)
    {
        x = part.x_min;
    }
    else if(curr_x > part.x_max) 
    {
        x = part.x_max;
    }
    else
    {
        x = curr_x;
    }
    
    if(curr_y < part.y_min)
    {
        y = part.y_min;
    }
    else if(curr_y > part.y_max) 
    {
        y = part.y_max;
    }
    else
    {
        y = curr_y;
    }
}

void NetList::quadratic_placement_iter(NetPart &part, double x_val, bool gt)
{
    std::vector<int> R, C;
    std::vector<double> V;
    
    size_t sz = part.e_idx - part.s_idx;
    int nnz = 0;
    
    std::vector<int>::iterator it;
    valarray<double> bx(sz);
    valarray<double> by(sz);
    
    SPGate so, so_neighbor;
    int gateID;
    int n_idx;
    
    double prop_x, prop_y;
    double a_weight;
    double pads_x;
    double pads_y;
    
    for(int idx = part.s_idx; idx < part.e_idx; ++idx)
    {
        so = gates[idx];
        gateID = so->gateID;
        
        a_weight = 0.0;
        pads_x   = 0.0;
        pads_y   = 0.0;
        
        for(it = so->conn_gates.begin() ; it != so->conn_gates.end(); ++it)
        { 
            so_neighbor = gates_map[*it];
            n_idx = so_neighbor->idx;
            
            if(n_idx < part.s_idx || n_idx >= part.e_idx)
            {
                propagate_gate(part, so_neighbor->x, so_neighbor->y, prop_x, prop_y);
                pads_x += prop_x;  
                pads_y += prop_y;
            }
            else
            {
                R.push_back(idx - part.s_idx);
                C.push_back(n_idx - part.s_idx);
                V.push_back(-edge_weight);
                ++nnz; 
            }    
            
            a_weight += edge_weight;
        }
        
        for(it = so->conn_pads.begin() ; it != so->conn_pads.end(); ++it)
        { 
            propagate_gate(part, pinX[*it], pinY[*it], prop_x, prop_y);
            pads_x += prop_x;  
            pads_y += prop_y;
            
            a_weight += edge_weight;
        }
        
        if(a_weight != 0)
        {
            R.push_back(idx - part.s_idx);
            C.push_back(idx - part.s_idx);
            V.push_back(a_weight);
            ++nnz;
            
            bx[idx - part.s_idx] = pads_x; 
            by[idx - part.s_idx] = pads_y;
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
    
    for(int idx=part.s_idx; idx < part.e_idx; ++idx)
    {
        so = gates[idx];
        
        so->x = x[idx - part.s_idx];
        so->y = y[idx - part.s_idx];
    }
    
    std::cout << "fin" << std::endl;
}

void NetList::quadratic_placement(int depth)
{
    int sz = gates.size();
    int cp = 1;
    
    SPNetPart spa, spb;
    SPNetPart sp = parts.front();
    
    quadratic_placement_iter(*sp, 100);
    sort_gates(*sp, true);
    
    spa = SPNetPart(new NetPart());
    spb = SPNetPart(new NetPart());
    
    sp->split_horizontal(*spa, *spb);
    //parts.erase(parts.begin());
    
    quadratic_placement_iter(*spa, 50.0, false);
    quadratic_placement_iter(*spb, 50.0, true );
    
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

int main(int argc, char * argv[])
{
    if(argc != 2)
    {
        std::cerr << "usage: 3qp [input name]" << std::endl;
        return -1;
    }

    NetList nl(argv[1]);
    nl.quadratic_placement();

    return 0;
}
