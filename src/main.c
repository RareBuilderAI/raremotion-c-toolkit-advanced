#include "project.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "data/projects.txt"

static int read_number(const char *prompt) {
    char input[64];
    char *end;
    long value;

    printf("%s", prompt);

    if (!fgets(input, sizeof(input), stdin))
        return -1;

    value = strtol(input, &end, 10);

    if (end == input)
        return -1;

    while (*end == ' ' || *end == '\t')
        end++;

    if (*end != '\n' && *end != '\0')
        return -1;

    if (value < 0 || value > 1000000)
        return -1;

    return (int)value;
}

static void add_new_project(ProjectList *list) {
    char name[PROJECT_NAME_MAX];

    printf("Enter project name: ");

    if (!fgets(name, sizeof(name), stdin))
        return;

    char *newline = strchr(name, '\n');

    if (newline) {
        *newline = '\0';
    } else {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        puts("Project name is too long.");
        return;
    }

    int id = add_project(list, name);

    if (id)
        printf("Project created successfully! ID: %d\n", id);
    else
        puts("Unable to add project. Check name or duplicates.");
}

int main(void) {
    ProjectList projects;
    init_projects(&projects);

    load_projects(&projects, DATA_FILE);

    int running = 1;

    while (running) {
        puts("\n================================");
        puts("   RAREMOTION ADVANCED C TOOLKIT");
        puts("================================");
        puts("1. View Projects");
        puts("2. Add Project");
        puts("3. Complete Project");
        puts("4. Delete Project");
        puts("5. Project Statistics");
        puts("6. Save Projects");
        puts("0. Save and Exit");

        int choice = read_number("\nChoose an option: ");

        switch (choice) {
            case 1:
                list_projects(&projects);
                break;

            case 2:
                add_new_project(&projects);
                break;

            case 3: {
                int id = read_number("Project ID: ");
                puts(complete_project(&projects, id)
                     ? "Project completed!"
                     : "Project not found.");
                break;
            }

            case 4: {
                int id = read_number("Project ID: ");
                puts(delete_project(&projects, id)
                     ? "Project deleted!"
                     : "Project not found.");
                break;
            }

            case 5:
                show_statistics(&projects);
                break;

            case 6:
                puts(save_projects(&projects, DATA_FILE)
                     ? "Projects saved!"
                     : "Unable to save projects.");
                break;

            case 0:
                if (save_projects(&projects, DATA_FILE)) {
                    puts("Projects saved. Goodbye!");
                    running = 0;
                } else {
                    puts("Save failed. Please try again.");
                }
                break;

            default:
                puts("Invalid option. Try again.");
        }
    }

    free_projects(&projects);
    return 0;
}
