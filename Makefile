CC = clang
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

APP = raremotion
TEST = test_project

.PHONY: all run test sanitize clean

all: $(APP)

$(APP): src/main.c src/project.c include/project.h
	$(CC) $(CFLAGS) src/main.c src/project.c -o $(APP)

run: $(APP)
	./$(APP)

$(TEST): tests/test_project.c src/project.c include/project.h
	$(CC) $(CFLAGS) tests/test_project.c src/project.c -o $(TEST)

test: $(TEST)
	./$(TEST)

sanitize:
	$(CC) $(CFLAGS) -g -fsanitize=address,undefined \
	-fno-omit-frame-pointer tests/test_project.c src/project.c \
	-o test_project_sanitized
	./test_project_sanitized

clean:
	rm -f $(APP) $(TEST) test_project_sanitized
