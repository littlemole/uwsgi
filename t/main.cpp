
#include "gtest/gtest.h"
#include "cryptics.h"

int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    ERR_remove_state(0);
    
    ERR_free_strings();
    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();
    return r;
}

