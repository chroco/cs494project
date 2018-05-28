#ifndef _TEST_H_
#define _TEST_H_

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "node.h"
#include "dll.h"
#include "channel.h"
#include "irc.h"
#include "server.h"
#include "client.h"

#define TEST_ASSERT_PRINT(FUNC, TYPE, RES, SHOULD_BE) \
    dprintf(STDERR_FILENO, \
       "%s: " #FUNC " was %" TYPE " but should have been %" TYPE "\n", __func__, \
        RES, SHOULD_BE); \

#define TEST_ASSERT_UINT16_EQ(FUNC, SHOULD_BE) { \
  uint16_t res = FUNC; \
  if (res != SHOULD_BE) { \
    TEST_ASSERT_PRINT(FUNC, PRIi16, res, SHOULD_BE) \
    return EXIT_FAILURE; \
  }}

#define TEST_ASSERT_UINT32_EQ(FUNC, SHOULD_BE) { \
  uint32_t res = FUNC; \
  if (res != SHOULD_BE) { \
    TEST_ASSERT_PRINT(FUNC, PRIi32, res, SHOULD_BE) \
    return EXIT_FAILURE; \
  }}

#define TEST_ASSERT_INT_EQ(FUNC, SHOULD_BE) { \
  int res = FUNC; \
  if (res != SHOULD_BE) { \
    TEST_ASSERT_PRINT(FUNC, "d", res, SHOULD_BE) \
    return EXIT_FAILURE; \
  }}

#define TEST_ASSERT_PTR_EQ(FUNC, SHOULD_BE) { \
  void *res = FUNC; \
  if (res != SHOULD_BE) { \
    dprintf(STDERR_FILENO, \
       "%s: " #FUNC " pointers should be equal\n", __func__); \
    return EXIT_FAILURE; \
  }}

#define TEST_ASSERT_CSTRING_EQ(FUNC, SHOULD_BE,SIZE) { \
  char *res = FUNC; \
  if (strncmp(SHOULD_BE,res,SIZE) { \
    dprintf(STDERR_FILENO, \
       "%s: " #FUNC " cstrings should be equal\n", __func__); \
    return EXIT_FAILURE; \
  }}

#define TEST_ASSERT_INT_NOT(FUNC, SHOULDNT_BE) { \
  int res = FUNC; \
  if (res == SHOULDNT_BE) { \
    dprintf(STDERR_FILENO, \
       "%s: " #FUNC " was %d but should not have been\n", __func__, \
        res); \
    return EXIT_FAILURE; \
  }}

#define TEST_ASSERT_NOT_NULL(FUNC) { \
  void *res = FUNC; \
  if (res == NULL) { \
    dprintf(STDERR_FILENO, \
       "%s: " #FUNC " should not have been NULL\n", __func__); \
    return EXIT_FAILURE; \
  }}

#define TEST_ASSERT(FUNC) { \
  if (!FUNC) { \
    dprintf(STDERR_FILENO, \
       "%s: " #FUNC " should not have been NULL\n", __func__); \
    return EXIT_FAILURE; \
  }}

int test_compiler();


class Test{
	public:
		Test();
		~Test();
		// unit tests
		int test_Node();
		int test_DLL();
		int test_DLL_createNode();
		int test_wordSerialization();
		int test_charArraySerialization();
		int test_packetSerialization();
		int test_serializeIRCPacket();
		int test_deserializeIRCPacket();
		int test_addClient();
		// integration tests
		int test_IRC();
		int test_Server();
		int test_Client();
		int test_DLL_insertNode();
		int test_DLL_searchNode();
		int test_Channel_searchNode();
		int test_Channel_searchName();
		int test_Client_searchNode();
		int test_Client_searchName();
		int test_DLL_Node();

	private:

};

#endif /* _TEST_H_ */
