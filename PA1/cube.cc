#include "cube.hh"

int Cube::size = -1;

Cube::Cube()
{
    _arr  = new boolean_t[Cube::size];
    std::fill_n(_arr, Cube::size, DCARE);
}

Cube::Cube(std::string str)
{
    std::istringstream iss(str);
	int sz, idx, i=0;
	
    _arr  = new boolean_t[Cube::size];
    std::fill_n(_arr, Cube::size, DCARE);
    
    iss >> sz;
	while(i++ < sz)
	{
		iss >> idx;
 
		if(idx < 0)
			_arr[(-idx) - 1] = NEG;
		else
			_arr[  idx  - 1] = POS;
	}
}

Cube::Cube(const Cube &rhs)
{
    _arr  = new boolean_t[Cube::size];
    std::copy(rhs._arr, rhs._arr + Cube::size, _arr);
}

bool Cube::is_1() const
{
    for(int i=0; i < Cube::size; ++i)
    {
        if(_arr[i] != DCARE)
            return false;
    }
    
    return true;
}

Cube & Cube::operator=(const Cube &rhs)
{
    if (this == &rhs){
      	return *this;
	}
    
    std::copy(rhs._arr, rhs._arr + Cube::size, _arr);

	return *this;
}
 
void Cube::print() const
{
    for(int i=0; i < Cube::size; ++i)
    {
        std::cout << (i+1) << ") " << _arr[i] << std::endl;
    }
}
