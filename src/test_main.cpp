#include "test.h"

int main(int argc,char *argv[]){
	Test *pTest = new Test();
	// *********** Unit Testing **************** //
	TEST_ASSERT_INT_EQ(pTest->test_Node(), EXIT_SUCCESS);
	TEST_ASSERT_INT_EQ(pTest->test_DLL_createNode(), EXIT_SUCCESS);
	TEST_ASSERT_INT_EQ(pTest->test_DLL_insertNode(), EXIT_SUCCESS);
	TEST_ASSERT_INT_EQ(pTest->test_DLL_searchNode(), EXIT_SUCCESS);
	TEST_ASSERT_INT_EQ(pTest->test_Channel_searchNode(), EXIT_SUCCESS);
	// ********* Integration Testing *********** //
	printf("All tests passed!\n");
	delete pTest;
	return 0;
}
