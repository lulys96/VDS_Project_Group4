#include <cassert>
#include "Manager.h"

// Local Variables:
// mode: c++
// End:

using namespace ClassProject; 
Manager::Manager()
{
    uni_table.resize(2);
    uni_table[1].high=1;
    uni_table[1].low=1;
    uni_table[1].top_var=1;
}