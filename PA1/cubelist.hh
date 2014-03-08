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

    void Cofactor(CubeList &cl, cofactor_t type, int idx);
 
public:
    CubeList() {}
    CubeList(std::string fname);
    
    ~CubeList() {}
    
    void addCube(SPCube so);
    void AND(int idx, boolean_t type);
    
    bool isEmpty() const;
    bool oneCube() const;
    
    SPCube operator[](int i) const {return _lst[i];} 
    CubeList & operator=(const CubeList &rhs);
    
    //CubeList Complement();
    CubeList positiveCofactor(int idx); 
    CubeList negativeCofactor(int idx);
    
    void print();
};

void Complement(CubeList &cl, const Cube &c);
CubeList Complement(const CubeList &cl);

#endif // _CUBELIST_HH_
