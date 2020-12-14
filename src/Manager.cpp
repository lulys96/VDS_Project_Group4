#include <cassert>
#include "Manager.h"

// Local Variables:
// mode: c++
// End:

using namespace ClassProject; 
Manager::Manager()
{
    //TableEntry new_node = TableEntry();
    uni_table.push_back(TableEntry()); //use push_back function
    uni_table[0].id=0;
    uni_table[0].high=0;
    uni_table[0].low=0;
    uni_table[0].top_var=0;
    uni_table[0].label="0";

    uni_table.push_back(TableEntry());
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
    TableEntry new_node = TableEntry();
    new_node.label = ""; //no label yet
    new_node.high = t;
    new_node.low = e;
    new_node.id = uniqueTableSize();
    new_node.top_var = i;
    uni_table.push_back(new_node);
    return new_node.id;
}

std::string Manager::getTopVarName(const BDD_ID &root)
{
    BDD_ID topVar_ID = topVar(root);
    return uni_table[topVar_ID].label;
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
    for (auto& it : uni_table) {
        if (it.label == label) return it.id;
    }
    
    BDD_ID varID = uniqueTableSize();
    BDD_ID newID = ite(varID,1,0);
    uni_table[newID].label = label;
    return newID;
}

BDD_ID Manager::coFactorTrue(const BDD_ID f)
{


}

BDD_ID Manager::coFactorFalse(const BDD_ID f)
{
    for (auto& it : uni_table) {
        if (it.id == f)
            if(it.id == False())
                return 0;
            else
                return 1;
        else
            std::out_of_range("No existing entry for variable!!!");
    }

}

BDD_ID Manager::and2(const BDD_ID a, const BDD_ID b)
{
    BDD_ID newID = ite(a,b,0);
    uni_table[newID].label = "and";
    return newID;
}
