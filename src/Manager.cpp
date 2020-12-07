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
    if (i) return t;
    else return e;
}

std::string Manager::getTopVarName(const BDD_ID &root)
{
    return uni_table[root].label;
}

BDD_ID Manager::topVar(const BDD_ID f) 
{
    return uni_table[f].top_var;
}

void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root)
{
    nodes_of_root.insert(uni_table[root].id);
    nodes_of_root.insert(uni_table[root].high);
    nodes_of_root.insert(uni_table[root].low);
}

BDD_ID Manager::createVar(const std::string &label)
{
    TableEntry new_var = TableEntry();
    new_var.label = label;
    new_var.high = 1;
    new_var.low = 0;
    new_var.id = uniqueTableSize();
    new_var.top_var = new_var.id;
    uni_table.push_back(new_var);
    return new_var.id;
}

BDD_ID Manager::and2(const BDD_ID a, const BDD_ID b)
{

}
