
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#define _TEST_LIST

#include "list.h"

int cnt;

void list_c(void** state){
    book bk = {0};
    int i;
    strcpy(bk.str, "test");
    assert_null(getHead());
    for(i = 0; i < cnt; i++){
        pushBack(&bk);
        assert_non_null(getHead());
        assert_string_equal(getAt(i)->bk.str, "test");
    }
    assert_ptr_equal(getHead(), getAt(0));
    while(i--){
        assert_true(remove_at(i));
    }
    assert_false(remove_at(i));   
    assert_ptr_equal(getHead(), getAt(0));
}

int main(int argc, char* argv[]){
    if(argc > 1){
        cnt = atoi(argv[1]);
    } 
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(list_c),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
