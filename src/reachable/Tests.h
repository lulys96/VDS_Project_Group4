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

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    auto s2 = states.at(2);
    ASSERT_EQ(s0,comp.False());
    ASSERT_EQ(s1,comp.False());
    ASSERT_EQ(s2,comp.False());
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
    BDD_ID a=5, b=10;
    ASSERT_THROW(comp.xnor2(a,1), std::out_of_range);
    ASSERT_THROW(comp.xnor2(1,b), std::out_of_range);
    ASSERT_THROW(comp.xnor2(a,b), std::out_of_range);

    BDD_ID idA = comp.createVar("a");
    BDD_ID idB = comp.createVar("b");
    BDD_ID xorID1 = comp.xnor2(idA,idA);
    ASSERT_EQ(comp.uniqueTableSize(), 4);

    BDD_ID xorID2 = comp.xnor2(0,idB);
    ASSERT_EQ(comp.uniqueTableSize(), 5);

    BDD_ID xorID3 = comp.xnor2(idA,0);
    BDD_ID xorID4 = comp.xnor2(idA,1);
    BDD_ID xorID5 = comp.xnor2(1,idB);
    BDD_ID xorID6 = comp.xnor2(idA,idB);
    BDD_ID xorID7 = comp.xnor2(idB,idA);

    //00/11 -> true     01/10 -> false
    ASSERT_EQ(xorID1, comp.True());
    ASSERT_EQ(xorID2, comp.neg(idB));
    ASSERT_EQ(xorID3, comp.neg(idA));
    ASSERT_EQ(xorID4, idA);
    ASSERT_EQ(xorID5, idB);
    ASSERT_EQ(comp.coFactorTrue(xorID6), idB);
    ASSERT_EQ(comp.coFactorFalse(xorID6), comp.neg(idB));

    ASSERT_TRUE(xorID6==xorID7);
    ASSERT_TRUE(comp.getTopVarName(xorID6)=="a");
    ASSERT_TRUE(comp.topVar(xorID6)==idA);
}

TEST(managerTest, HowTo_Example) {

    ClassProject::Reachable comp(2);

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    //s0' = not(s0)
    // functions.push_back(comp.neg(s0));
    // //s1' = not(s1)
    // functions.push_back(comp.neg(s0));
    // //Add transition functions
    // comp.setDelta(functions);
    // //Add init state
    // comp.setInitState({false,false});

    // ASSERT_TRUE(comp.is_reachable({true,true}));
    // ASSERT_TRUE(comp.is_reachable({false,false}));
    // ASSERT_FALSE(comp.is_reachable({true,false}));
    // ASSERT_FALSE(comp.is_reachable({false,true}));

}



#endif //VDSPROJECT_TESTS_H
