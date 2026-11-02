#include "project.h"

#include <assert.h>
#include <stdio.h>

static void test_add_project(void) {
    ProjectList list;
    init_projects(&list);

    assert(add_project(&list, "Raremotion CRM") == 1);
    assert(list.count == 1);
    assert(list.items[0].completed == 0);

    free_projects(&list);
    puts("PASS: Add project");
}

static void test_duplicate_project(void) {
    ProjectList list;
    init_projects(&list);

    assert(add_project(&list, "Raremotion API") == 1);
    assert(add_project(&list, "Raremotion API") == 0);
    assert(list.count == 1);

    free_projects(&list);
    puts("PASS: Duplicate prevention");
}

static void test_complete_project(void) {
    ProjectList list;
    init_projects(&list);

    int id = add_project(&list, "Raremotion Inventory");

    assert(complete_project(&list, id) == 1);
    assert(list.items[0].completed == 1);
    assert(complete_project(&list, 999) == 0);

    free_projects(&list);
    puts("PASS: Complete project");
}

static void test_delete_project(void) {
    ProjectList list;
    init_projects(&list);

    int id = add_project(&list, "Raremotion Analytics");

    assert(delete_project(&list, id) == 1);
    assert(list.count == 0);
    assert(delete_project(&list, id) == 0);

    free_projects(&list);
    puts("PASS: Delete project");
}

static void test_save_and_load(void) {
    const char *filename = "data/test_projects.txt";

    ProjectList original;
    ProjectList loaded;

    init_projects(&original);
    init_projects(&loaded);

    int id = add_project(&original, "Raremotion Document AI");
    assert(id == 1);
    assert(complete_project(&original, id) == 1);
    assert(save_projects(&original, filename) == 1);

    assert(load_projects(&loaded, filename) == 1);
    assert(loaded.count == 1);
    assert(loaded.items[0].completed == 1);

    remove(filename);

    free_projects(&original);
    free_projects(&loaded);

    puts("PASS: Save and load");
}

int main(void) {
    test_add_project();
    test_duplicate_project();
    test_complete_project();
    test_delete_project();
    test_save_and_load();

    puts("\nAll 5 tests passed!");
    return 0;
}
