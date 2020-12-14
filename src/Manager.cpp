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

bool Manager::isConstant(const BDD_ID f)
{
    if(f == False() || f == True()) //if terminal node return true
        return true;
    else return false;
}

bool Manager::isVariable(const BDD_ID x)
{
    if(uni_table[x].low == False() && uni_table[x].high  == True() 
                                   && uni_table[x].top_var  == x)
        return true;
    else return false;
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
    for (auto& it : uni_table) {
        if ((it.top_var == i) && (it.high == t) && (it.low == e) ) {
            return it.id;
        }
    }

    BDD_ID T = ite(coFactorTrue(i),coFactorTrue(t),coFactorTrue(e));

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

    if (uni_table[root].high > 1) {
        findNodes(uni_table[root].high, nodes_of_root );
    }

    if (uni_table[root].low > 1) {
        findNodes(uni_table[root].low, nodes_of_root );
    }
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
    for (auto& it : uni_table) {
        if (it.id == f)
             if(it.id == True())
                return 1;
            else
                return it.high;
        else
            std::out_of_range("No existing entry for variable!!!");
    }
}

BDD_ID Manager::coFactorFalse(const BDD_ID f)
{
    for (auto& it : uni_table) {
        if (it.id == f)
            if(it.id == False())
                return 0;
            else
                return it.low;
        else
            std::out_of_range("No existing entry for variable!!!");
    }
}

BDD_ID Manager::coFactorTrue(const BDD_ID f, BDD_ID x)
{
if(isConstant(x) || isConstant(f) || uni_table[f].top_var > x)
        return f;
if(topVar(f) == x)
    return uni_table[f].high;
else
    {
    BDD_ID    T = coFactorTrue(uni_table[f].high, x);
    BDD_ID    F = coFactorTrue(uni_table[f].low, x);
    return ite(uni_table[f].top_var, T, F);
    }
}

BDD_ID Manager::coFactorFalse(const BDD_ID f, BDD_ID x)
{
if(isConstant(x) || isConstant(f) || uni_table[f].top_var > x)
        return f;
if(topVar(f) == x)
    return uni_table[f].low;
else
    {
    BDD_ID    T = coFactorFalse(uni_table[f].high, x);
    BDD_ID    F = coFactorFalse(uni_table[f].low, x);
    return ite(uni_table[f].top_var, T, F);
    }
}


BDD_ID Manager::and2(const BDD_ID a, const BDD_ID b)
{   
    //topVar is terminal node
    if (isConstant(a)) {
        if (a==0) return a;
        else return b;
    }
    else {
    BDD_ID newID = ite(a,b,0);
    return newID;
    }
}

BDD_ID Manager::or2(const BDD_ID a, const BDD_ID b)
{
    if(isConstant(a))
        if(a == 1) 
            return 1;
        else 
            return b;
    else if(isConstant(b))
            if(b == 1)
                return 1;
            else
                return a;
    else if(a == b)
        return a;
    else
        return ite(a,1,b);          
}
