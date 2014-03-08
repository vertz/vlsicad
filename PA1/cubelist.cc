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

void CubeList::Cofactor(CubeList &cl, cofactor_t type, int idx)
{
    SPCube so;
    
    std::vector<SPCube>::iterator it;
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

CubeList CubeList::positiveCofactor(int idx)
{
    CubeList cl;
    Cofactor(cl, POSFACTOR, idx);
    return cl;
}

CubeList CubeList::negativeCofactor(int idx)
{
    CubeList cl;
    Cofactor(cl, NEGFACTOR, idx);
    return cl;
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

bool CubeList::isEmpty() const
{
    return _lst.empty();
}

bool CubeList::oneCube() const
{
    return _lst.size() == 1;
}

void CubeList::print()
{
    std::vector<SPCube>::iterator it;
    for (it = _lst.begin() ; it != _lst.end(); ++it)
    {
        std::cout << "########" << std::endl;
        (*it)->print();
    }
    
    std::cout << "<<<>>>" << std::endl;
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
                break;
            case POS:
                so = SPCube(new Cube());
                (*so)[i] = NEG;
                break;
            default:
                continue;    
        }
    
        cl.addCube(so);
    }
}

CubeList Complement(const CubeList &cl)
{
    CubeList ret;

    if(cl.isEmpty())
    {
        SPCube so = SPCube(new Cube());
        ret.addCube(so);
        return ret;
    }   
    else if(cl.oneCube())
    {
        Complement(ret, *cl[0]);
        return ret;
    }
    
    return ret;
}

int main()
{
    CubeList cl0("test.txt");
    cl0.print();
    
    CubeList cl1 = cl0.positiveCofactor(0);
    cl1.print();
    
    CubeList cl2 = Complement(cl1);
    cl2.print();
    
    return 0;
}
