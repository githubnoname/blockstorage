#ifndef TESTUTILS
#define TESTUTILS

#include<functional>
#include<stdio.h>


#define TEST(name) test_func(#name, name)

#define ERROR(...) do {                                                 \
        fprintf(stderr, "%s:%d ", __FILE__, __LINE__);                  \
        fprintf(stderr, __VA_ARGS__);                                   \
        fprintf(stderr, "\n");                                          \
        return false; }                                                 \
    while(0)

inline bool test_func(std::string const &aDesc, std::function<bool()> aFunc){
    auto result = aFunc();
    printf("%s - %s\n", aDesc.c_str(), result ? "OK" : "FAILED");
    return result;
}

#endif
