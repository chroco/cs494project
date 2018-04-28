#include "test.h"

int main(int argc,char *argv[]){
	Test *pTest = new Test();
	// *********** Unit Testing **************** //
	TEST_ASSERT_INT_EQ(pTest->testNode(), EXIT_SUCCESS);
	// ********* Integration Testing *********** //
	printf("All tests passed!\n");
	delete pTest;
	return 0;
}
