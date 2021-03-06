#ifndef _CUBELIST_HH_
#define _CUBELIST_HH_

#include <algorithm>
#include <fstream>
#include <vector>
#include <cmath> 
#include "cube.hh"

#include <boost/shared_ptr.hpp>

class CubeList
{
private:
    std::vector<SPCube> _lst;

    enum cofactor_t
    { 
        POSFACTOR, 
        NEGFACTOR 
    };

    void Cofactor(CubeList &cl, cofactor_t type, int idx) const;
 
public:
    CubeList() {}
    CubeList(const CubeList &rhs);
    CubeList(std::string fname);
    
    ~CubeList() {}
    
    void addCube(SPCube so);
    void AND(int idx, boolean_t type);
    
    bool isEmpty() const;
    bool oneCube() const;
    bool contain_1() const;
    
    SPCube operator[](int i) const {return _lst[i];} 
    CubeList & operator=(const CubeList &rhs);
    
    int size() const;
    int most_binate() const;
    
    //CubeList Complement();
    void positiveCofactor(CubeList &cl, int idx) const; 
    void negativeCofactor(CubeList &cl, int idx) const;
};

void Complement(CubeList &cl, const Cube &c);
void Complement(CubeList &cl, const CubeList &rhs);

void OR(CubeList &cl, const CubeList &lhs, const CubeList &rhs);
void AND(CubeList &cl, const CubeList &lhs, const CubeList &rhs);

//# Boost Shared Pointer for CubeList
typedef boost::shared_ptr<CubeList> SPCubeList;

#endif // _CUBELIST_HH_
