#include "list.h"
#include <assert.h>

int main(int argc, char* argv[]){
    book bk = {0};
    int i;
    strcpy(bk.str, "test");
    for(i = 0; i < atoi(argv[1]); i++){
        pushBack(&bk);
        assert(!strcmp(getAt(i)->bk.str, "test"));
    }
    while(i--){
        assert(remove_at(i) == 1);
    }
    assert(remove_at(i) == 0);   
    return 0;
}
