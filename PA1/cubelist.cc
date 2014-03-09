#include "cubelist.hh"

CubeList::CubeList(std::string fname)
{
    std::ifstream f(fname.c_str());
    std::string line;
    SPCube so;
    
    if(f.fail()){
        std::cerr << "couldn't open the file (\"" << fname << "\")" << std::endl;
        return;
    }
    
    f >> Cube::size;
    
    // remove the first two lines :/
    getline(f, line);
    getline(f, line);
    
    while(getline(f, line))
    {
        so = SPCube(new Cube(line));
        _lst.push_back(so);
    }
    
    f.close();
}

CubeList::CubeList(const CubeList &rhs)
{
    std::copy(rhs._lst.begin(), rhs._lst.end(), _lst.begin());
}

CubeList & CubeList::operator=(const CubeList &rhs)
{
    if (this == &rhs){
      	return *this;
	}
    
    std::copy(rhs._lst.begin(), rhs._lst.end(), _lst.begin());

	return *this;
}

void CubeList::addCube(SPCube so)
{
    _lst.push_back(so);
}

void CubeList::Cofactor(CubeList &cl, cofactor_t type, int idx) const
{
    SPCube so;
    
    std::vector<SPCube>::const_iterator it;
    for (it = _lst.begin() ; it != _lst.end(); ++it)
    {
        so = SPCube(new Cube(**it));
        
        switch((*so)[idx])
        {
            case POS:
                if(type == POSFACTOR)
                {
                    (*so)[idx] = DCARE;
                    cl.addCube(so);
                }
                break;
            case NEG:
                if(type == NEGFACTOR)
                {
                    (*so)[idx] = DCARE;
                    cl.addCube(so);
                }
                break;
            default:
                cl.addCube(so);
                break;
        }  
    }
}

void CubeList::positiveCofactor(CubeList &cl, int idx) const
{
    Cofactor(cl, POSFACTOR, idx);
}

void CubeList::negativeCofactor(CubeList &cl, int idx) const
{
    Cofactor(cl, NEGFACTOR, idx);
}

void CubeList::AND(int idx, boolean_t type)
{
    boolean_t complement = (type == POS) ? NEG : POS;

    std::vector<SPCube>::iterator it;
    for (it = _lst.begin() ; it != _lst.end(); ++it)
    {
        if((**it)[idx] == complement)
        {
            _lst.erase(it);
            --it;
        }
        else
        {
            (**it)[idx] = type;
        }
    }
}

int CubeList::size() const
{
    return _lst.size();
}

bool CubeList::isEmpty() const
{
    return _lst.empty();
}

bool CubeList::oneCube() const
{
    return _lst.size() == 1;
}

bool CubeList::contain_1() const
{
    std::vector<SPCube>::const_iterator it;
    for (it = _lst.begin() ; it != _lst.end(); ++it)
    {
        if((*it)->is_1())
            return true;
    }
    
    return false;
}

int CubeList::most_binate() const
{
    int pos[Cube::size], neg[Cube::size];
    std::fill_n(pos, Cube::size, 0);
    std::fill_n(neg, Cube::size, 0);
    
    std::vector<SPCube>::const_iterator it;
    for (it = _lst.begin() ; it != _lst.end(); ++it){
        for(int i=0 ; i < Cube::size; ++i)
        {
            switch((**it)[i])
            {
                case NEG:
                    ++neg[i];
                    break;
                case POS:
                    ++pos[i];
                    break;
                default:
                    continue;    
            }
        }
    }
    
    int idx = -1, max_cubes = -1, max_binate = -1;
    int n_cubes, binate;
    
    for(int i=0 ; i < Cube::size; ++i)
    {
        if(pos[i] > 0 && neg[i] > 0)
        {
            n_cubes = pos[i] + neg[i];
            if(n_cubes > max_cubes)
            {
                max_cubes  = n_cubes;
                max_binate = std::abs(pos[i] - neg[i]);
                idx = i;
            }
            else if(n_cubes == max_cubes)
            {
                binate = std::abs(pos[i] - neg[i]);
                if(binate > max_binate)
                {
                    max_binate = binate;
                    idx = i;
                }
            }
        }
    }
    
    if(idx < 0)
    {
        for(int i=0 ; i < Cube::size; ++i)
        {
            n_cubes = pos[i] + neg[i];
            if(n_cubes > max_cubes)
            {
                max_cubes  = n_cubes;
                idx = i;
            }
        } 
    }
    
    return idx;
}

void OR(CubeList &cl, const CubeList &lhs, const CubeList &rhs)
{
    for (int i=0 ; i < lhs.size() ; ++i)
    {
        cl.addCube(lhs[i]);
    }
    
    for (int i=0 ; i < rhs.size() ; ++i)
    {
        cl.addCube(rhs[i]);
    }
}

void Complement(CubeList &cl, const Cube &c)
{
    SPCube so;
    
    for(int i=0; i < Cube::size; ++i)
    {
        switch(c[i])
        {
            case NEG:
                so = SPCube(new Cube());
                (*so)[i] = POS;
                cl.addCube(so);
                break;
            case POS:
                so = SPCube(new Cube());
                (*so)[i] = NEG;
                cl.addCube(so);
                break;
            default:
                continue;    
        }
    }
}

CubeList Complement(const CubeList &cl)
{
    CubeList ret;

    if(cl.oneCube())
    {
        Complement(ret, *cl[0]);
        return ret;
    }   
    else if(cl.isEmpty())
    {
        // empty represents the Boolean equation “0”
        // the complement is clearly “1”
        // new cube initialized to [11 11 ... 11]
        // represents "1"
        SPCube so = SPCube(new Cube());
        ret.addCube(so);
        return ret;
    }
    else if(cl.contain_1())
    {
        // (x1 + ... + 1 + .. + xn) = 1
        // F = 1 --> F' = 0
        // return an empty cubeList
        return ret;
    }
    
    // most binate variable for splitting
    int idx = cl.most_binate();
    
    CubeList pos_cl, neg_cl;
    
    cl.positiveCofactor(pos_cl, idx);
    CubeList p_comp = Complement(pos_cl);
    
    cl.negativeCofactor(neg_cl, idx);
    CubeList n_comp = Complement(neg_cl);
    
    p_comp.AND(idx, POS);
    n_comp.AND(idx, NEG);
    
    OR(ret, p_comp, n_comp);
    
    return ret;
}

