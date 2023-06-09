
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define START_SIZE 10

typedef struct Stree {
    char * word;
    unsigned int count;
    struct Stree * left;
    struct Stree * right;
} tree;

char * read_string(FILE * file) {
    int i = 0, size = START_SIZE, buf;
    char * string = (char * ) malloc(START_SIZE * sizeof(char));
    char * new_string = string;
    for (;; new_string = (char * ) realloc(string, (size *= 2) * sizeof(char))) {
        if (new_string == NULL) {
            free(string);
            return NULL;
        } else {
            string = new_string;
            for (; i < size;) {
                buf = fgetc(file);
                if ((buf == '\n') || (buf == EOF)) {
                    string[i] = '\0';
                    if ((i == 0) && (buf == EOF)) return NULL;
                    new_string = (char * ) realloc(string, (i + 1) * sizeof(char));
                    if (new_string == NULL) free(string);
                    return new_string;
                } else {
                    string[i++] = buf;
                }
            }
        }
    }
}
 

tree * find_branch(char * word, tree * root) {
    if (root == NULL) return NULL;
    tree * found;
    int difference = strcmp(word, root -> word);

    if (difference > 0) {
        if (root -> right != NULL)
            return find_branch(word, root -> right);
        else
            return root;
    } else if (difference < 0) {
        if (root -> left != NULL)
            return find_branch(word, root -> left);
        else
            return root;
    } else
        return root;
}


tree * construct_leaf(char * word) {
    tree * leaf = (tree * ) malloc(sizeof(tree));
    leaf -> word = word;
    leaf -> count = 1;
    leaf -> left = NULL;
    leaf -> right = NULL;
    return leaf;
}

void add_leaf(char * word, tree * root, tree * leaf) {
    tree * found_branch = find_branch(word, root);
    int difference = strcmp(word, found_branch -> word);
    if (difference > 0) {
        found_branch -> right = leaf;
    } else if (difference < 0) {
        found_branch -> left = leaf;
    } else {
        (found_branch -> count) ++;
    }
}


void delete_tree(tree * root) {
    if (root == NULL) return;
    delete_tree(root -> left);
    delete_tree(root -> right);
    free(root);
}

void print_tree_upper(tree * root) {
    if (root == NULL) return;
    print_tree_upper(root -> left);
    printf("%s %d\n", root -> word, root -> count);
    print_tree_upper(root -> right);
}

void print_tree_lower(tree * root) {
    if (root == NULL) return;
    print_tree_lower(root -> right);
    printf("%s %d\n", root -> word, root -> count);
    print_tree_lower(root -> left);
}

int main(int argc, char ** argv) {

    char * string;
    FILE * file;
    int n = 1;
    tree * root;
    // проверка количества аргументов камандной строки
    if (argc < 2){
        puts("Incorect number of arguments\n\
        Exemple: .\\main.exe file.txt");
        exit(1);
    }  

    //  когда аргемнтов 2 то есть задана каманда .\main.exe file.txt
    if (argc == 2){
       
        file = fopen(argv[1], "r");
        // проверка на открытие файла
        if (file == NULL){
            puts("Error: invalid openning file.\n\
            Exemple: .\\main.exe file.txt ");
            exit(1);
        }
        if ((string = read_string(file)) != NULL) {
        tree * root = construct_leaf(string);
        while ((string = read_string(file)) != NULL) {
            tree * new_leaf = construct_leaf(string);
            add_leaf(string, root, new_leaf);
        }
        print_tree_upper(root); // вывод по умолчанию
        exit(0);
    }
    }

    if (argc != 4){
        puts("Error: Incorect number of arguments.");
        puts("Exemple: .\\main.exe -o DESC file.txt");
        exit(1);
    }
if ((strcmp(argv[1],"-o") != 0 && strcmp(argv[2],"DESC") != 0) || (strcmp(argv[1],"-o") != 0 && strcmp(argv[2],"ASC") != 0)){
    puts("Incorrect input of options\n\
    Exemple: .\\main.exe -o DESC file.txt");
    exit(1);
}
    file = fopen(argv[3], "r");
    if (file == NULL){
        puts("Error: opennig file.");
        exit(1);
    }

    for (int i = 1; i <= argc - 2; i++) {
        if (!strcmp(argv[i], "-o")) n = strcmp(argv[i + 1], "DESC");
    }
    if ((string = read_string(file)) != NULL) {
        root = construct_leaf(string);
        while ((string = read_string(file)) != NULL) {
            tree * new_leaf = construct_leaf(string);
            add_leaf(string, root, new_leaf);
        }
        if (n)
            print_tree_upper(root);
        else
            print_tree_lower(root);
        delete_tree(root);
    }
    fclose(file);
    return 0;
}

