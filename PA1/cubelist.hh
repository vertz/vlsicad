#ifndef _CUBELIST_HH_
#define _CUBELIST_HH_

#include <exception>
#include <fstream>
#include <vector>
#include "cube.hh"

class CubeList
{
private:
    std::vector<SPCube> _lst;

    enum cofactor_t
    { 
        POSFACTOR, 
        NEGFACTOR 
    };

    void addCube(SPCube so);
    void Cofactor(CubeList &cl, cofactor_t type, int idx);
 
public:
    CubeList() {}
    CubeList(std::string fname);
    
    ~CubeList() {}
    
    bool isEmpty() const;
    
    //CubeList Complement();
    CubeList positiveCofactor(int idx); 
    CubeList negativeCofactor(int idx);
    
    CubeList & operator=(const CubeList &rhs);
    
    void AND(int idx, boolean_t type);
    void print();
};

#endif // _CUBELIST_HH_
