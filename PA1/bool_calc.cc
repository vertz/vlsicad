#include "cubelist.hh"
#include <iostream> 
#include <fstream>
#include <string>
#include <map>

void output_op(SPCubeList so, int idx)
{
    std::stringstream ss;
    std::string fname;

    ss << idx << ".pcn" << std::endl;
    getline(ss, fname);

    std::ofstream g(fname);
    if (g.is_open())
    {
        g << Cube::size   << std::endl;
        g << (*so).size() << std::endl;
        
        for(int i=0; i < (*so).size(); ++i)
        {
            g << (*(*so)[i]).to_string() << std::endl;
        }
        
        g.close();
    }
}

int main(int argc, char * argv[])
{
    if(argc != 2)
    {
        std::cerr << "usage: bool_calc [input name]" << std::endl;
        return -1;
    }

    std::ifstream f(argv[1]);
    if(f.fail()){
        std::cerr << "couldn't open the file (\"" << argv[1] << "\")" << std::endl;
        return -1;
    }
    
    std::map<int, SPCubeList> funcs;
    
    std::stringstream ss;
    std::string fname;
    
    int x1, x2, x3;
    SPCubeList so;
    char op;
    
    while(1)
    {
        f >> op;
    
        switch(op)
        {
            case 'r':
                f >> x1;
                ss << x1 << ".pcn" << std::endl;
                getline(ss, fname);
                so = SPCubeList(new CubeList(fname));
                funcs[x1] = so;
                break;
            
            case '!':
                f >> x1 >> x2;
                so = SPCubeList(new CubeList());
                Complement(*so, *funcs[x2]);
                funcs[x1] = so;
                break;
                
            case '+':
                f >> x1 >> x2 >> x3;
                so = SPCubeList(new CubeList());
                OR(*so, *funcs[x2], *funcs[x3]);
                funcs[x1] = so;
                break;
                
            case '&':
                f >> x1 >> x2 >> x3;
                so = SPCubeList(new CubeList());
                AND(*so, *funcs[x2], *funcs[x3]);
                funcs[x1] = so;
                break;    
                
            case 'p':
                f >> x1;
                output_op(funcs[x1], x1);
                break;
                
            case 'q':
                return 0;
                
            default:
                std::cerr << "unknown operator (" << op << ")" << std::endl;
                std::cerr << "legal operators [r/!/+/&/p/q]"   << std::endl;
                return -1;
        }
    }
    
    return 0;
}
