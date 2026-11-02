#ifndef PROJECT_H
#define PROJECT_H

#include <stddef.h>

#define PROJECT_NAME_MAX 100

typedef struct {
    int id;
    char name[PROJECT_NAME_MAX];
    int completed;
} Project;

typedef struct {
    Project *items;
    size_t count;
    size_t capacity;
} ProjectList;

void init_projects(ProjectList *list);
void free_projects(ProjectList *list);

int add_project(ProjectList *list, const char *name);
int delete_project(ProjectList *list, int id);
int complete_project(ProjectList *list, int id);

void list_projects(const ProjectList *list);
void show_statistics(const ProjectList *list);

int save_projects(const ProjectList *list, const char *filename);
int load_projects(ProjectList *list, const char *filename);

#endif
