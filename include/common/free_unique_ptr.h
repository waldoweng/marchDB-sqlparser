#ifndef FREE_UNIQUE_PTE_H__
#define FREE_UNIQUE_PTE_H__

#include <cstdlib>
#include <memory>


struct free_deleter {
public:
    void operator() (const char *ptr) const {
        if (ptr) {
            printf("%s", ptr);
            free(const_cast<char *>(ptr));
        }
    }
};

typedef std::unique_ptr<const char, free_deleter> free_unique_ptr;

#endif
