#include <cassert>
#include "Manager.h"

// Local Variables:
// mode: c++
// End:

using namespace ClassProject; 
Manager::Manager()
{
    uni_table.resize(2);
    uni_table[0].label="0";
    uni_table[1].id=1;
    uni_table[1].high=1;
    uni_table[1].low=1;
    uni_table[1].top_var=1;
    uni_table[1].label="1";
}
const BDD_ID &Manager::True()
{
    if ((uni_table[1].high == 1) &&
        (uni_table[1].low == 1) &&
        (uni_table[1].top_var == 1)) {
            return uni_table[1].id;
    }
    throw std::out_of_range("Terminal node 1 wrongly initialized"); 
}

const BDD_ID &Manager::False()
{
    if ((uni_table[0].high == 0) &&
        (uni_table[0].low == 0) &&
        (uni_table[0].top_var == 0)) { 
            return uni_table[0].id;
    }
    else throw std::out_of_range("Terminal node 0 wrongly initialized"); 
}

size_t Manager::uniqueTableSize()
{
    return uni_table.size();
}

BDD_ID Manager::ite(const BDD_ID i, const BDD_ID t, const BDD_ID e)
{
    
}