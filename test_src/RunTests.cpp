#include <gtest/gtest.h>
#include "Enemy.h"
#include "Slingshot.h"

/// <summary>
///Taken from the GoogleTest primer. 
/// </summary>

// The fixture for testing class Foo.
class EnemyTest : public testing::Test {
public:
    std::unique_ptr<Enemy> enemy;
  
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.


    EnemyTest() {
        // You can do set-up work for each test here.
          
    }

    ~EnemyTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
        enemy = std::make_unique<Enemy>(50); // All enemnies in this test suite start with 50 HP
                    
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }
   

};


//class ParamTest : public::testing::TestWithParam<int> {
//protected:
//    ParamTest() = default;
//    ~ParamTest() = default;
//
//    void SetUp() override {
//        // Code here will be called immediately after the constructor 
//    }
//
//    void TearDown() override {
//    }
//};
//TEST_P(ParamTest, SimpleTest) {
//    int i_test = GetParam();
//    std::cout << "Param value:: " << i_test << std::endl;
//    EXPECT_GT(i_test, 1);
//}
//INSTANTIATE_TEST_SUITE_P(
//    Simple,
//    ParamTest, ::testing::Values(1, 2, 3, 4, 5)
//);
//








//A single test, not a fixture. No setup is called.
TEST(Enemy, First_test) {
    Enemy e(100);
    EXPECT_EQ(e.getHealth(), 100);
    //SUCCEED() << "Test test passed";
   // FAIL() << "Test didn't pass";
}

TEST_F(EnemyTest, LethalDamagePopsPig) {
    enemy->takeDamage(60);
    EXPECT_TRUE(enemy->checkIfPopped());
}

TEST(EnemyTest1, XPosition_Test) {
	Enemy e(0);
	EXPECT_EQ(e.getX(), 0);
}

TEST(EnemyTest2, YPosition_Test) {
    Enemy e(0);
    EXPECT_EQ(e.getY(), 0);
}

TEST(SlingshotTest, tension_Test) {
    Slingshot s;
	EXPECT_EQ(s.getTension(), 0);
}

TEST(SlingshotTest, pullBack_Test) {
    Slingshot s;
    EXPECT_TRUE(s.pullBack(30));
    EXPECT_EQ(s.getTension(), 30);
}



int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}



