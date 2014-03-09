#include "cubelist.hh"
#include <iostream> 
#include <fstream>

int main(int argc, char * argv[])
{
    if(argc != 2)
    {
        std::cerr << "usage: urp [input name]" << std::endl;
        return -1;
    }

    CubeList cl(argv[1]);
    CubeList comp_cl = Complement(cl);
    
    std::ofstream f("sol.pcn");
    if (f.is_open())
    {
        f << Cube::size     << std::endl;
        f << comp_cl.size() << std::endl;
        
        for(int i=0; i < comp_cl.size(); ++i)
        {
            f << (*comp_cl[i]).to_string() << std::endl;
        }
        
        f.close();
    }
    
    return 0;
}
