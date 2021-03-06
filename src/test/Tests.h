//
// Created by tobias on 21.11.16.
//

#ifndef VDS_PROJECT_GTESTS_H
#define VDS_PROJECT_GTESTS_H

#include "gtest/gtest.h"
#include "../Manager.h"
#include "../ManagerInterface.h"

using namespace ClassProject;
class ManagerTest : public ::testing::Test {
protected:
    Manager my_manager;
};

TEST_F(ManagerTest, CtorTableSize) 
{
    ASSERT_TRUE(my_manager.uniqueTableSize()==2);
}

TEST_F(ManagerTest, isConstant) 
{
    BDD_ID idF = my_manager.createVar("x1");
    ASSERT_EQ(my_manager.isConstant(0), true);
    ASSERT_EQ(my_manager.isConstant(1), true);
    ASSERT_EQ(my_manager.isConstant(idF), false);
}

TEST_F(ManagerTest, isVariable)
{
    ASSERT_EQ(my_manager.isVariable(0), false);
    ASSERT_EQ(my_manager.isVariable(1), false);
    ASSERT_EQ(my_manager.isVariable(my_manager.createVar("x1")), true);
    ASSERT_EQ(my_manager.isVariable(my_manager.createVar("x2")), true);
    ASSERT_EQ(my_manager.isVariable(my_manager.and2(2,3)), false); //and node is not a var
} 

TEST_F(ManagerTest, CtorTableTermNodes) 
{
    EXPECT_NO_THROW(my_manager.True());
    ASSERT_TRUE(my_manager.True()==1);
    EXPECT_NO_THROW(my_manager.False());
    ASSERT_TRUE(my_manager.False()==0); 
} 

TEST_F(ManagerTest, coFactorFalse)
{
    BDD_ID f = 10;
    ASSERT_THROW(my_manager.coFactorFalse(f), std::out_of_range);
    ASSERT_FALSE(my_manager.coFactorFalse(0));
    ASSERT_TRUE(my_manager.coFactorFalse(1));

    f = my_manager.createVar("x1");
    ASSERT_TRUE(my_manager.coFactorFalse(f)==0);
}

TEST_F(ManagerTest, coFactorPositiv)
{
    BDD_ID f = 5;
    ASSERT_THROW(my_manager.coFactorTrue(f), std::out_of_range);
    ASSERT_FALSE(my_manager.coFactorTrue(0));
    ASSERT_TRUE(my_manager.coFactorTrue(1));

    f = my_manager.createVar("x1");
    ASSERT_TRUE(my_manager.coFactorTrue(f)==1);
 }

TEST_F(ManagerTest, coFactorPositiv2)
{
    BDD_ID f = 5,x;
    ASSERT_THROW(my_manager.coFactorTrue(f,x), std::out_of_range);
    ASSERT_EQ(my_manager.coFactorTrue(0,0), 0);
    ASSERT_EQ(my_manager.coFactorTrue(1,1), 1);
    ASSERT_EQ(my_manager.coFactorTrue(1,0), 1);
    ASSERT_EQ(my_manager.coFactorTrue(0,1), 0);

    f = my_manager.createVar("x1");
    ASSERT_EQ(my_manager.coFactorTrue(f,0), f);
    ASSERT_EQ(my_manager.coFactorTrue(f,1), f);
    ASSERT_EQ(my_manager.coFactorTrue(1,f), 1);
    ASSERT_EQ(my_manager.coFactorTrue(0,f), 0);
    ASSERT_EQ(my_manager.coFactorTrue(f,f), 1);
}

TEST_F(ManagerTest, coFactorNegative2)
{
    BDD_ID f = 5,x;
    ASSERT_THROW(my_manager.coFactorFalse(f,x), std::out_of_range);
    ASSERT_EQ(my_manager.coFactorFalse(0,0), 0);
    ASSERT_EQ(my_manager.coFactorFalse(1,1), 1);
    ASSERT_EQ(my_manager.coFactorFalse(1,0), 1);
    ASSERT_EQ(my_manager.coFactorFalse(0,1), 0);

    f = my_manager.createVar("x1");
    ASSERT_EQ(my_manager.coFactorFalse(f,0), f);
    ASSERT_EQ(my_manager.coFactorFalse(f,1), f);
    ASSERT_EQ(my_manager.coFactorFalse(1,f), 1);
    ASSERT_EQ(my_manager.coFactorFalse(0,f), 0);
    ASSERT_EQ(my_manager.coFactorFalse(f,f), 0);
}

TEST_F(ManagerTest, createVar) 
{
//NEXT_ID_ava(id) - "x1"(label) - NEXT_ID_ava(TopVar) - 1(high) - 0(low)
    my_manager.createVar("x1");
    my_manager.createVar("x1"); // try to create an already existing var
    ASSERT_TRUE(my_manager.uniqueTableSize()==3);
    ASSERT_TRUE(my_manager.getTopVarName(2)=="x1");
    std::set<BDD_ID> nodes;
    my_manager.findNodes(2,nodes);
    const bool is_in = (nodes.find(0) != nodes.end()) &
                       (nodes.find(1) != nodes.end()) &
                       (nodes.find(2) != nodes.end());
    ASSERT_TRUE(is_in);
    ASSERT_TRUE(my_manager.topVar(2)==2);

}

TEST_F(ManagerTest, ite_terminal_var)
{
    ASSERT_EQ(my_manager.ite (0,0,1),1);
    ASSERT_EQ(my_manager.ite (0,1,0),0);
    ASSERT_EQ(my_manager.ite (1,1,0),1);
    ASSERT_EQ(my_manager.ite (0,1,0),0);
    ASSERT_EQ(my_manager.ite (1,0,1),0);
    ASSERT_EQ(my_manager.uniqueTableSize(),2);
    BDD_ID idA = my_manager.createVar("a");
    ASSERT_EQ(my_manager.ite (idA,1,0),2);
    ASSERT_EQ(my_manager.uniqueTableSize(),3);
    BDD_ID idB = my_manager.createVar("b");
    ASSERT_EQ(my_manager.ite (idA,idB,idB),idB);
    ASSERT_EQ(my_manager.ite (idA,idA,0),idA); //AND(idA,idA)=idA
}

TEST_F(ManagerTest, ite_cofactor)
{
    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    //And function
    BDD_ID ID_AND_A_B = my_manager.ite (idA,idB,my_manager.False()); 
    ASSERT_EQ(ID_AND_A_B,4); //only checks if creates the id, might not be sufficient 
    ASSERT_EQ(my_manager.topVar(ID_AND_A_B),idA);
    ASSERT_EQ(my_manager.coFactorFalse(ID_AND_A_B),my_manager.False());
    ASSERT_EQ(my_manager.coFactorTrue(ID_AND_A_B),idB);
}

TEST_F(ManagerTest, findNodes)
{
    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    BDD_ID idC = my_manager.createVar("c");
    BDD_ID idD = my_manager.createVar("d");
    BDD_ID andID1 = my_manager.ite (idA,idB,0);
    BDD_ID andID2 = my_manager.ite (idC,andID1,0);

    BDD_ID andID3= my_manager.ite (idA,idD,0);

    std::set<BDD_ID> nodes;
    my_manager.findNodes(andID2,nodes);

    ASSERT_EQ((nodes.find(0) != nodes.end()),true);
    ASSERT_EQ((nodes.find(1) != nodes.end()),true);
    ASSERT_EQ((nodes.find(7) != nodes.end()),true); //ID=7,topVar=B,low=0,high=C (created to be andID2_high)
    ASSERT_EQ((nodes.find(idC) != nodes.end()),true);  //high of id7
    ASSERT_EQ((nodes.find(andID2) != nodes.end()),true);  

    ASSERT_EQ((nodes.find(andID1) != nodes.end()),false); // not added directly because it needs to be ordered
    ASSERT_EQ((nodes.find(andID3) != nodes.end()),false); // different operation
    ASSERT_EQ((nodes.find(idA) != nodes.end()),false);  //top var of andID2 and and ID1
    ASSERT_EQ((nodes.find(idB) != nodes.end()),false); //top var of id7
    ASSERT_EQ((nodes.find(idD) != nodes.end()),false);  //not related to andID2
}

TEST_F(ManagerTest, findVars_terminal)
{
    std::set<BDD_ID> var_false;
    my_manager.findVars(0,var_false);
    ASSERT_EQ((var_false.find(0) != var_false.end()),false);

    std::set<BDD_ID> var_true;
    my_manager.findVars(1,var_true);
    ASSERT_EQ((var_true.find(1) != var_true.end()),false);
}
TEST_F(ManagerTest, findVars)
{   
    //TODO find more cases to test
    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b"); // add test case where only var is tested, should only return the tested var
    BDD_ID idC = my_manager.createVar("c"); 
    BDD_ID idD = my_manager.createVar("d");
    BDD_ID andID1 = my_manager.ite (idA,idB,0);
    BDD_ID andID2 = my_manager.ite (idC,andID1,0);

    BDD_ID andID3= my_manager.ite (idA,idD,0);

    std::set<BDD_ID> vars;
    my_manager.findVars(andID2,vars);
    
    ASSERT_EQ((vars.find(idA) != vars.end()),true);  
    ASSERT_EQ((vars.find(idB) != vars.end()),true);  
    ASSERT_EQ((vars.find(idC) != vars.end()),true);  

    ASSERT_EQ((vars.find(andID1) != vars.end()),false);  
    ASSERT_EQ((vars.find(andID2) != vars.end()),false);
    ASSERT_EQ((vars.find(idD) != vars.end()),false);
    ASSERT_EQ((vars.find(0) != vars.end()),false);
    ASSERT_EQ((vars.find(1) != vars.end()),false);
    ASSERT_EQ((vars.find(7) != vars.end()),false);
}

TEST_F(ManagerTest, neg_terminal)
{   
    BDD_ID negID1 = my_manager.neg(0);
    BDD_ID negID2 = my_manager.neg(1);
            
    ASSERT_TRUE(negID1==1);  
    ASSERT_TRUE(negID2==0);
}

TEST_F(ManagerTest, neg)
{   
    BDD_ID a=5;
    ASSERT_THROW(my_manager.neg(a), std::out_of_range);

    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    BDD_ID negA = my_manager.neg(idA);
    BDD_ID negA2 = my_manager.neg(idA);
    BDD_ID negB = my_manager.neg(idB);

    ASSERT_EQ(negA2,negA);
    ASSERT_NE(my_manager.neg(idA),idA);
    ASSERT_EQ(my_manager.neg(negA),idA);

    BDD_ID andID = my_manager.ite(idA,idB,0);
    BDD_ID nandID = my_manager.neg(andID);

    ASSERT_NE(nandID, andID);
    ASSERT_EQ(my_manager.neg(nandID),andID);
}


TEST_F(ManagerTest, or2_terminal)
{
    BDD_ID orID1 = my_manager.or2(0,0);
    BDD_ID orID2 = my_manager.or2(1,1);
    BDD_ID orID3 = my_manager.or2(1,0);
    BDD_ID orID4 = my_manager.or2(0,1);    

    ASSERT_TRUE(orID1 == 0);
    ASSERT_TRUE(orID2 == 1); 
    ASSERT_TRUE(orID3 == 1);
    ASSERT_TRUE(orID4 == 1);
}

TEST_F(ManagerTest, or2_var)
{
    BDD_ID a=2, b=10;
    ASSERT_THROW(my_manager.or2(a,1), std::out_of_range);
    ASSERT_THROW(my_manager.or2(1,b), std::out_of_range);
    ASSERT_THROW(my_manager.or2(a,b), std::out_of_range);

    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    BDD_ID orID1 = my_manager.or2(0,idA);
    BDD_ID orID2 = my_manager.or2(1,idA);
    BDD_ID orID3 = my_manager.or2(idA,0);
    BDD_ID orID4 = my_manager.or2(idA,1);
    BDD_ID orID5 = my_manager.or2(idA,idA);
    BDD_ID orID6 = my_manager.or2(idA,idB);
    BDD_ID orID7 = my_manager.or2(idB,idA);

    ASSERT_EQ(orID1, idA);
    ASSERT_EQ(orID2, 1);
    ASSERT_EQ(orID3, idA);
    ASSERT_EQ(orID4, 1);
    ASSERT_EQ(orID5, idA);
    ASSERT_EQ(orID6, 4);
    ASSERT_EQ(orID7, 4);
}

TEST_F(ManagerTest, xor2_terminal)
{
    BDD_ID xorID1 = my_manager.xor2(0,0);
    BDD_ID xorID2 = my_manager.xor2(1,1);
    BDD_ID xorID3 = my_manager.xor2(1,0);
    BDD_ID xorID4 = my_manager.xor2(0,1);    
    ASSERT_EQ(my_manager.uniqueTableSize(), 2);

    ASSERT_EQ(xorID1, 0);
    ASSERT_EQ(xorID2, 0); 
    ASSERT_EQ(xorID3, 1);
    ASSERT_EQ(xorID4, 1);
}

TEST_F(ManagerTest, xor2_var)
{
    BDD_ID a=5, b=10;
    ASSERT_THROW(my_manager.xor2(a,1), std::out_of_range);
    ASSERT_THROW(my_manager.xor2(1,b), std::out_of_range);
    ASSERT_THROW(my_manager.xor2(a,b), std::out_of_range);

    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    BDD_ID xorID1 = my_manager.xor2(idA,idA);
    ASSERT_EQ(my_manager.uniqueTableSize(), 4);

    BDD_ID xorID2 = my_manager.xor2(0,idB);
    ASSERT_EQ(my_manager.uniqueTableSize(), 5);

    BDD_ID xorID3 = my_manager.xor2(idA,0);
    BDD_ID xorID4 = my_manager.xor2(idA,1);
    BDD_ID xorID5 = my_manager.xor2(1,idB);
    BDD_ID xorID6 = my_manager.xor2(idA,idB);
    BDD_ID xorID7 = my_manager.xor2(idB,idA);

    ASSERT_EQ(xorID1, 0);
    ASSERT_EQ(xorID2, idB);
    ASSERT_EQ(xorID3, idA);
    ASSERT_EQ(xorID4, my_manager.neg(idA));
    ASSERT_EQ(xorID5, my_manager.neg(idB));
    ASSERT_EQ(my_manager.coFactorTrue(xorID6), my_manager.neg(idB));
    ASSERT_EQ(my_manager.coFactorFalse(xorID6), idB);

    ASSERT_EQ(xorID6,6);
    ASSERT_TRUE(xorID6==xorID7);
    ASSERT_TRUE(my_manager.getTopVarName(xorID6)=="a");
    ASSERT_TRUE(my_manager.topVar(xorID6)==2);
}

TEST_F(ManagerTest, and2_terminals)
{   
    BDD_ID andID1 = my_manager.and2(0,0);
    BDD_ID andID2 = my_manager.and2(0,1);
    BDD_ID andID3 = my_manager.and2(1,0);
    BDD_ID andID4 = my_manager.and2(1,1);
            
    ASSERT_TRUE(andID1==0);  
    ASSERT_TRUE(andID2==0);
    ASSERT_TRUE(andID3==0);
    ASSERT_TRUE(andID4==1);
}

TEST_F(ManagerTest, and2)
{   
    BDD_ID a=5, b=10;
    ASSERT_THROW(my_manager.and2(a,1), std::out_of_range);
    ASSERT_THROW(my_manager.and2(1,b), std::out_of_range);
    ASSERT_THROW(my_manager.and2(a,b), std::out_of_range);

    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    BDD_ID andID = my_manager.and2(idA,idB);
    BDD_ID andID2 = my_manager.and2(idA,idB);
    std::set<BDD_ID> nodes;
    my_manager.findNodes(andID,nodes);
    const bool is_in = (nodes.find(andID) != nodes.end()) &
                       (nodes.find(0) != nodes.end()) &
                       (nodes.find(idB) != nodes.end());
    
    ASSERT_EQ(my_manager.uniqueTableSize(),5);
    ASSERT_EQ(andID,4);
    ASSERT_EQ(andID2,4);
    ASSERT_TRUE(my_manager.getTopVarName(andID)=="a");
    ASSERT_EQ(my_manager.topVar(andID),2);
    ASSERT_TRUE(is_in);
    //TODO: extend tests to work with ands of ands
}

TEST_F(ManagerTest, nand2_terminals)
{   
    BDD_ID andID1 = my_manager.nand2(0,0);
    BDD_ID andID2 = my_manager.nand2(0,1);
    BDD_ID andID3 = my_manager.nand2(1,0);
    BDD_ID andID4 = my_manager.nand2(1,1);
            
    ASSERT_TRUE(andID1==1);  
    ASSERT_TRUE(andID2==1);
    ASSERT_TRUE(andID3==1);
    ASSERT_TRUE(andID4==0);
}

TEST_F(ManagerTest, nand2)
{   
    BDD_ID a=5, b=10;
    ASSERT_THROW(my_manager.nand2(a,1), std::out_of_range);
    ASSERT_THROW(my_manager.nand2(1,b), std::out_of_range);
    ASSERT_THROW(my_manager.nand2(a,b), std::out_of_range);

    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    BDD_ID nandID = my_manager.nand2(idA,idB);
    BDD_ID nandID2 = my_manager.nand2(idA,idB);
    std::set<BDD_ID> nodes;
    my_manager.findNodes(nandID,nodes);
    const bool is_in = (nodes.find(nandID) != nodes.end()) &
                       (nodes.find(1) != nodes.end()) &
                       (nodes.find(0) != nodes.end()) &
                       (nodes.find(4) != nodes.end()); //node representing neg(b) createb by nand
    
    ASSERT_EQ(my_manager.uniqueTableSize(),6);
    ASSERT_EQ(nandID,5);
    ASSERT_TRUE(nandID==nandID2);
    ASSERT_TRUE(my_manager.getTopVarName(nandID)=="a");
    ASSERT_EQ(my_manager.topVar(nandID),2);
    ASSERT_TRUE(is_in);
    //TODO: extend tests to work with nands of nands
}


TEST_F(ManagerTest, nor2_terminal)
{
    BDD_ID norID1 = my_manager.nor2(0,0);
    BDD_ID norID2 = my_manager.nor2(1,1);
    BDD_ID norID3 = my_manager.nor2(1,0);
    BDD_ID norID4 = my_manager.nor2(0,1);    

    ASSERT_EQ(norID1, 1);
    ASSERT_EQ(norID2, 0); 
    ASSERT_EQ(norID3, 0);
    ASSERT_EQ(norID4, 0);

    ASSERT_EQ(my_manager.uniqueTableSize(),2);
}

TEST_F(ManagerTest, nor2_var)
{
    BDD_ID a=2, b=10;
    ASSERT_THROW(my_manager.nor2(a,1), std::out_of_range);
    ASSERT_THROW(my_manager.nor2(1,b), std::out_of_range);
    ASSERT_THROW(my_manager.nor2(a,b), std::out_of_range);

    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    BDD_ID norID1 = my_manager.nor2(idA,idA);
    BDD_ID norID2 = my_manager.nor2(0,idB);
    BDD_ID norID3 = my_manager.nor2(idA,0);
    BDD_ID norID4 = my_manager.nor2(idA,1);
    BDD_ID norID5 = my_manager.nor2(1,idB);
    BDD_ID norID6 = my_manager.nor2(idA,idB);
    BDD_ID norID7 = my_manager.nor2(idB,idA);

    ASSERT_EQ(norID1, my_manager.neg(idA));
    ASSERT_EQ(norID2, my_manager.neg(idB));
    ASSERT_EQ(norID3, my_manager.neg(idA));
    ASSERT_EQ(norID4, 0);
    ASSERT_EQ(norID5, 0);
    
    ASSERT_EQ(my_manager.coFactorTrue(norID6), 0);
    ASSERT_EQ(my_manager.coFactorFalse(norID6), my_manager.neg(idB));

    ASSERT_EQ(norID6,6);
    ASSERT_TRUE(norID6==norID7);
    ASSERT_TRUE(my_manager.getTopVarName(norID6)=="a");
    ASSERT_TRUE(my_manager.topVar(norID6)==2);
}


TEST_F(ManagerTest, topVarFromSet)
{
    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    BDD_ID idC = my_manager.createVar("c");
    BDD_ID idD = my_manager.createVar("d");
    BDD_ID idAnd = my_manager.and2(idD,idB);
    BDD_ID idAnd2 = my_manager.and2(idB,idC);

    BDD_ID topVariable = my_manager.topVarFromSet(idB,1,idA);
    BDD_ID topVariable1 = my_manager.topVarFromSet(1,idB,idA);
    BDD_ID topVariable2 = my_manager.topVarFromSet(idA,1,idB);
    BDD_ID topVariable3 = my_manager.topVarFromSet(idC,idA,idB);
    BDD_ID topVariable4 = my_manager.topVarFromSet(idAnd,idAnd2,idD);


    ASSERT_EQ(topVariable, idA);
    ASSERT_EQ(topVariable1, idA);
    ASSERT_EQ(topVariable2, idA);
    ASSERT_EQ(topVariable3, idA);
    ASSERT_EQ(topVariable4, idB);

}

TEST_F(ManagerTest, deMorgan)
{
    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    BDD_ID idC = my_manager.createVar("c");

    
    BDD_ID negIdA = my_manager.neg(idA);
    BDD_ID negIdB = my_manager.neg(idB);


    BDD_ID or2 = my_manager.or2(negIdA,negIdB);
    BDD_ID nand2 = my_manager.nand2(idA,idB);

    BDD_ID nor2 = my_manager.nor2(idB, idA);
    BDD_ID and2 = my_manager.and2(negIdB, negIdA);
    BDD_ID idF = my_manager.and2(and2, nor2);

    BDD_ID topVariable = my_manager.topVarFromSet(or2,nor2,idA);
    BDD_ID topVariable2 = my_manager.topVarFromSet(or2,idF,nor2);

    ASSERT_EQ(or2,nand2);
    ASSERT_EQ(nor2,and2);
    ASSERT_EQ(my_manager.topVar(nand2),idA);
    ASSERT_EQ(my_manager.topVar(and2),idA);
    ASSERT_EQ(topVariable, idA);
    ASSERT_EQ(topVariable2, idA);

}

TEST_F (ManagerTest,deMorgan2)
{
    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    BDD_ID idC = my_manager.createVar("c");
    BDD_ID idD = my_manager.createVar("d");

    BDD_ID negIdA = my_manager.neg(idA);
    BDD_ID negIdB = my_manager.neg(idB);
    BDD_ID negIdC = my_manager.neg(idC);
    BDD_ID negIdD = my_manager.neg(idD);

    BDD_ID or_negA_C = my_manager.or2(negIdA,idC);
    BDD_ID or_B_negD = my_manager.or2(idB,negIdD);
    BDD_ID left = my_manager.nand2(or_negA_C,or_B_negD);

    BDD_ID and_A_negC = my_manager.and2(idA,negIdC);
    BDD_ID and_negB_D = my_manager.and2(negIdB,idD);
    BDD_ID right = my_manager.or2(and_A_negC,and_negB_D);

    ASSERT_EQ(left,right);
    
}


#endif //VDS_PROJECT_TESTS_H

