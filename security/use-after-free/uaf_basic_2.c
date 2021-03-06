// https://sensepost.com/blog/2017/linux-heap-exploitation-intro-series-used-and-abused-use-after-free/

#include <malloc.h>
#include <stdio.h>

typedef struct UAFME {
    void (*vulnfunc)();
} UAFME;

void good(){
    printf("I AM GOOD :)\n");
}

void bad(){
    printf("I AM BAD >:|\n");
}

void helper_call_goodfunc(UAFME *uafme){
    UAFME *private_uafme = uafme;
    private_uafme->vulnfunc = good;
    private_uafme->vulnfunc();
    // After some time coding, the developer forgets that
    // previously, it was assigned: 
    //      private_uafme = uafme 
    // now, trying to be tidy, free()'s the "private" pointer
    // which actually is the malloc1 pointer 
    free(private_uafme); // OOPS
}

int main(int argc, const char * argv[]){
    printf("[i] Allocating a chunk malloc1 holding a UAFME struct\n");
    UAFME *malloc1 = malloc(sizeof(UAFME));
    printf("[+] malloc1 at %p\n", malloc1);
    printf("[i] good at %p\n", good);
    printf("[i] bad at %p\n", bad);
    helper_call_goodfunc(malloc1);
    // malloc1 is now free

    printf("[i] Allocating a chunk malloc2 with 24(Assuming 64bit) byte size\n");
    long *malloc2 = malloc(0);
    printf("[i] Setting malloc2 to bad's pointer\n");
    *malloc2 = (long)bad;
    printf("[+] malloc2 at %p\n", malloc2);

    // Here is where the use-after-free happens
    // as we are using the free malloc1 which 
    // was populated with an integer
    printf("[i] Now calling malloc1 vulnfunc again...\n"); 
    malloc1->vulnfunc();
}
