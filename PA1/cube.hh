#ifndef _CUBE_HH_
#define _CUBE_HH_

#include <algorithm>
#include <iostream> 
#include <sstream>
#include <string>

#include <boost/shared_ptr.hpp>
 
enum boolean_t
{
    NEG     = 0,
    POS     = 1,
    DCARE   = 2
};
 
class Cube
{
private:
    boolean_t *_arr;
 
public:
    static int size;

    Cube();
    Cube(std::string str);
    Cube(const Cube &rhs);
    ~Cube() {delete[] _arr;}
 
    boolean_t   operator[](int i) const {return _arr[i];} 
    boolean_t & operator[](int i) {return _arr[i];}
 
    Cube & operator=(const Cube &rhs);
 
    void print() const;
};

//# Boost Shared Pointer for Cube
typedef boost::shared_ptr<Cube> SPCube;

#endif // _CUBE_HH_
