#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <ctype.h>

struct section 
{
    char* name;

    int count;

    char** keys;
    char** values;
};

char* readSection(FILE* fp) {
    fgetc(fp);// read first character - [
    char *str, ch;
    int len = 0, j = 1;
    str = (char*)malloc(sizeof(char));
    while ((ch = getc(fp)) != ']') {
        str[len] = ch;
        len++;
        j++;
        str = (char*)realloc(str, j * sizeof(char));
    }
    str[len] = '\0';
    fgetc(fp); //read last character - \n
    return str;
}

char* readKey(FILE* fp) {
    char *key, ch;
    int Klen = 0;
    int j = 1;
    key = (char*)malloc(sizeof(char));
    while ((ch = getc(fp)) != ' ') {
        key[Klen] = ch;
        Klen++;
        j++;
        key = (char*)realloc(key, j * sizeof(char));
    }
    key[Klen] = '\0';
    fgetc(fp);
    fgetc(fp);
    
    return key;
}

char* readValue(FILE* fp) {
    char* val, ch;
    int Vlen = 0;
    int j = 1;
    val = (char*)malloc(sizeof(char));
    while ((ch = getc(fp)) != '\n') {
        val[Vlen] = ch;
        Vlen++;
        j++;
        val = (char*)realloc(val, j * sizeof(char));
    }
    val[Vlen] = '\0';
    return val;
}


int validite(char* str) {
    int i = 0;
    while(str[i] != '\0') {
        if(!isalnum(str[i]) && str[i] != '-') {
            return 0;
        } 
        i++;
    } return 1;
}

struct section* readSections(char* filename, int* sCount) {
    struct section* sections = (struct section*)malloc(sizeof(struct section));

    FILE *fp;

    fp = fopen(filename, "r");

    if (fp == NULL) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        ungetc(ch, fp);
        switch(ch) {
            case '[': {
                char* str = readSection(fp);
                if(!validite(str)) {
                    printf("Invalid section identifier: %s\n", str);
                    return 0;
                }

                *sCount += 1;
                sections = (struct section*)realloc(sections, *sCount * sizeof(struct section));
                sections[*sCount-1].name = str;
                sections[*sCount-1].keys = (char**)malloc(sizeof(char*));
                sections[*sCount-1].values = (char**)malloc(sizeof(char*));
                sections[*sCount-1].count = 0;
                
                break;
            } case ',': {
                while((ch = getc(fp)) != '\n') {}
                break;
            } case '\n': {
                fgetc(fp);
                break;
            } default: {
                char* key = readKey(fp);

                if(!validite(key)) {
                    printf("Invalid key identifier: %s\n", key);
                    return 0;
                }

                char* val = readValue(fp);

                sections[*sCount-1].count += 1;
                sections[*sCount-1].keys = (char**)realloc(sections[*sCount-1].keys, sections[*sCount-1].count * sizeof(char*));
                sections[*sCount-1].values = (char**)realloc(sections[*sCount-1].values, sections[*sCount-1].count * sizeof(char*));
                sections[*sCount-1].keys[sections[*sCount-1].count-1] = key;
                sections[*sCount-1].values[sections[*sCount-1].count-1] = val;

                break;
            }
        }
    }

    fclose(fp);
    return sections;
}

//STRUCTS

void freeSections(struct section* sections, int c) {
    for(int i = 0; i < c; i++) {
        for(int j = 0; j < sections[i].count; j++) {
            free(sections[i].keys[j]);
            free(sections[i].values[j]);
        }
        free(sections[i].keys);
        free(sections[i].values);
        free(sections[i].name);
    }
    free(sections);
}

void printSections(struct section* sections, int c) {
    for(int i = 0; i < c; i++) {
        printf("SECTION - [%s]\n", sections[i].name);
        for(int j = 0; j < sections[i].count; j++) {
            printf("    key = %s, value = %s\n", sections[i].keys[j], sections[i].values[j]);
        }
        printf("\n");
    }
}


int findValue(struct section* sections, int c, const char* s, const char* k, char** res) {
    for(int i = 0; i < c; i++) {
        if(!strcmp(sections[i].name, s)) {
            for(int j = 0; j < sections[i].count; j++) {
                if(!strcmp(sections[i].keys[j], k)) {
                    *res = sections[i].values[j];
                    return 0;
                }
            }
            return -2;
        }
    }
    return -1;
}

int isNumber(char* str) {
    int i = 0;
    while(str[i] != '\0') {
        if(i == 0) {
            if(!isdigit(str[i]) && str[i] != '-') {
                return 0;
            }
        } else {
            if(!isdigit(str[i])) {
                return 0;
            }
        }
        i += 1;
    }
    return 1;
}
