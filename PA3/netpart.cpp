#include "netpart.h"

void NetPart::split_idx(NetPart &pa, NetPart &pb)
{
    pa.s_idx = s_idx;
    pb.e_idx = e_idx;
    
    int len     = (e_idx - s_idx) / 2;
    int mid_idx = s_idx + len;
    
    pa.e_idx = mid_idx;
    pb.s_idx = mid_idx;
}

void NetPart::split_horizontal(NetPart &pa, NetPart &pb)
{
    split_idx(pa, pb);

    double sz      = (x_max - x_min) / 2.0;
    double mid_val = x_min + sz;
    
    pa.x_min = x_min;
    pa.x_max = mid_val;
    pa.y_min = y_min;
    pa.y_max = y_max;
    
    pb.x_min = mid_val;
    pb.x_max = x_max;
    pb.y_min = y_min;
    pb.y_max = y_max;
}

void NetPart::split_vertical(NetPart &pa, NetPart &pb)
{
    split_idx(pa, pb);
    
    double sz      = (y_max - y_min) / 2.0;
    double mid_val = y_min + sz;
    
    pa.y_min = y_min;
    pa.y_max = mid_val;
    pa.x_min = x_min;
    pa.x_max = x_max;
    
    pb.y_min = mid_val;
    pb.y_max = y_max;
    pb.x_min = x_min;
    pb.x_max = x_max;
}

