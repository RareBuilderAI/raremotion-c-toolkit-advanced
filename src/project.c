#include "project.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_projects(ProjectList *list) {
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

void free_projects(ProjectList *list) {
    free(list->items);
    init_projects(list);
}

static int find_index(const ProjectList *list, int id) {
    for (size_t i = 0; i < list->count; i++) {
        if (list->items[i].id == id)
            return (int)i;
    }
    return -1;
}

int add_project(ProjectList *list, const char *name) {
    if (!name || !name[0] || strlen(name) >= PROJECT_NAME_MAX)
        return 0;

    for (size_t i = 0; i < list->count; i++) {
        if (strcmp(list->items[i].name, name) == 0)
            return 0;
    }

    if (list->count == list->capacity) {
        size_t new_capacity = list->capacity ? list->capacity * 2 : 4;

        Project *new_items = realloc(
            list->items, new_capacity * sizeof(Project)
        );

        if (!new_items)
            return 0;

        list->items = new_items;
        list->capacity = new_capacity;
    }

    int next_id = 1;

    for (size_t i = 0; i < list->count; i++) {
        if (list->items[i].id >= next_id)
            next_id = list->items[i].id + 1;
    }

    Project *project = &list->items[list->count++];
    project->id = next_id;
    strcpy(project->name, name);
    project->completed = 0;

    return next_id;
}

int delete_project(ProjectList *list, int id) {
    int index = find_index(list, id);

    if (index < 0)
        return 0;

    for (size_t i = (size_t)index; i + 1 < list->count; i++) {
        list->items[i] = list->items[i + 1];
    }

    list->count--;
    return 1;
}

int complete_project(ProjectList *list, int id) {
    int index = find_index(list, id);

    if (index < 0)
        return 0;

    list->items[index].completed = 1;
    return 1;
}

void list_projects(const ProjectList *list) {
    if (list->count == 0) {
        puts("No projects available.");
        return;
    }

    for (size_t i = 0; i < list->count; i++) {
        const Project *p = &list->items[i];

        printf("[%d] %s - %s\n",
               p->id,
               p->name,
               p->completed ? "Completed" : "In Progress");
    }
}

void show_statistics(const ProjectList *list) {
    size_t completed = 0;

    for (size_t i = 0; i < list->count; i++) {
        if (list->items[i].completed)
            completed++;
    }

    printf("Total: %zu\n", list->count);
    printf("Completed: %zu\n", completed);
    printf("In Progress: %zu\n", list->count - completed);
}

int save_projects(const ProjectList *list, const char *filename) {
    FILE *file = fopen(filename, "w");

    if (!file)
        return 0;

    for (size_t i = 0; i < list->count; i++) {
        const Project *p = &list->items[i];

        if (fprintf(file, "%d|%d|%s\n",
                    p->id, p->completed, p->name) < 0) {
            fclose(file);
            return 0;
        }
    }

    return fclose(file) == 0;
}

int load_projects(ProjectList *list, const char *filename) {
    FILE *file = fopen(filename, "r");

    if (!file)
        return 0;

    char line[256];

    while (fgets(line, sizeof(line), file)) {
        Project project;

        if (sscanf(line, "%d|%d|%99[^\n]",
                   &project.id,
                   &project.completed,
                   project.name) != 3)
            continue;

        int id = add_project(list, project.name);

        if (id) {
            Project *added = &list->items[list->count - 1];
            added->id = project.id;
            added->completed = project.completed;
        }
    }

    fclose(file);
    return 1;
}
