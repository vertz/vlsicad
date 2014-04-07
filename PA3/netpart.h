#ifndef _NETPART_HH_
#define _NETPART_HH_

#include <boost/shared_ptr.hpp>

class NetPart
{
private:
    void split_idx(NetPart &pa, NetPart &pb);

public:
    int    s_idx, e_idx;
    double x_min, x_max;
    double y_min, y_max;
    bool horizontal;

    NetPart() : x_min(-1), y_min(-1) {}
    ~NetPart() {}
    
    void split_horizontal(NetPart &pa, NetPart &pb);
    void split_vertical(NetPart &pa, NetPart &pb);
};

//# Boost Shared Pointer for NetPart
typedef boost::shared_ptr<NetPart> SPNetPart;

#endif // _NETPART_HH_
