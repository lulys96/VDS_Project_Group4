//
// Created by tobias on 29.11.18.
//

#ifndef VDSPROJECT_TESTS_H
#define VDSPROJECT_TESTS_H

#include "Reachable.h"
#include "gtest/gtest.h"
using namespace ClassProject;


TEST(managerTest, getState) {

    ClassProject::Reachable comp(3);

    auto states = comp.getStates();

    BDD_ID s0 = states.at(0);
    BDD_ID s1 = states.at(1);
    BDD_ID s2 = states.at(2);
    ASSERT_EQ(s0,2);
    ASSERT_EQ(s1,3);
    ASSERT_EQ(s2,4);

    ASSERT_TRUE(comp.getTopVarName(s0)=="s0");
    ASSERT_TRUE(comp.isVariable(s0));

    ASSERT_TRUE(comp.getTopVarName(s1)=="s1");
    ASSERT_TRUE(comp.isVariable(s1));

    ASSERT_TRUE(comp.getTopVarName(s2)=="s2");
    ASSERT_TRUE(comp.isVariable(s2));


}

TEST(managerTest, xnor_terminal) {

    ClassProject::Reachable comp(2);
    BDD_ID idXNOR00 = comp.xnor2(comp.False(),comp.False());
    BDD_ID idXNOR01 = comp.xnor2(comp.False(),comp.True());
    BDD_ID idXNOR10 = comp.xnor2(comp.True(),comp.False());
    BDD_ID idXNOR11 = comp.xnor2(comp.True(),comp.True());

    ASSERT_EQ(idXNOR00, comp.True());
    ASSERT_EQ(idXNOR01, comp.False());
    ASSERT_EQ(idXNOR10, comp.False());
    ASSERT_EQ(idXNOR00, comp.True());
}


TEST(managerTest, xor2_var)
{
    ClassProject::Reachable comp(2);
    BDD_ID a=7, b=10;
    ASSERT_THROW(comp.xnor2(a,1), std::out_of_range);
    ASSERT_THROW(comp.xnor2(1,b), std::out_of_range);
    ASSERT_THROW(comp.xnor2(a,b), std::out_of_range);


    auto states = comp.getStates();

    BDD_ID s0 = states.at(0);
    BDD_ID s1 = states.at(1);

    BDD_ID xorID1 = comp.xnor2(s0,s0);
    ASSERT_EQ(comp.uniqueTableSize(), 6);

    BDD_ID xorID2 = comp.xnor2(0,s1);
    ASSERT_EQ(comp.uniqueTableSize(), 7);

    BDD_ID xorID3 = comp.xnor2(s0,0);
    BDD_ID xorID4 = comp.xnor2(s0,1);
    BDD_ID xorID5 = comp.xnor2(1,s1);
    BDD_ID xorID6 = comp.xnor2(s0,s1);
    BDD_ID xorID7 = comp.xnor2(s1,s0);

    //00/11 -> true     01/10 -> false
    ASSERT_EQ(xorID1, comp.True());
    ASSERT_EQ(xorID2, comp.neg(s1));
    ASSERT_EQ(xorID3, comp.neg(s0));
    ASSERT_EQ(xorID4, s0);
    ASSERT_EQ(xorID5, s1);
    ASSERT_EQ(comp.coFactorTrue(xorID6), s1);
    ASSERT_EQ(comp.coFactorFalse(xorID6), comp.neg(s1));

    ASSERT_TRUE(xorID6==xorID7);
    ASSERT_TRUE(comp.getTopVarName(xorID6)=="s0");
    ASSERT_TRUE(comp.topVar(xorID6)==s0);
}


TEST(managerTest, HowTo_Example) {

    ClassProject::Reachable comp(2);

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    //s0' = not(s0)
    functions.push_back(comp.neg(s0));
    //s1' = not(s1)
    functions.push_back(comp.neg(s0));
    //Add transition functions
    comp.setDelta(functions);
    //Add init state

    comp.setInitState({false,false});

    ASSERT_TRUE(comp.is_reachable({true,true}));
    ASSERT_TRUE(comp.is_reachable({false,false}));
    ASSERT_FALSE(comp.is_reachable({true,false}));
    ASSERT_FALSE(comp.is_reachable({false,true}));

}



#endif //VDSPROJECT_TESTS_H
