#include "utils.h"

int main(int argc, char *argv[])
{

    if(argc < 3) {
        printf("Not enough arguments!");
        return ERROR_IN_ARGUMENTS;
    } else if (argc > 4) {
        printf("Too much arguments!");
        return ERROR_IN_ARGUMENTS;
    }

    int sCount = 0;
    struct section* dictionary = readSections(argv[1], &sCount);

    const char* delimiter = ".";
    
    if(argc == 3) {
        const char* s;
        const char* k;

        if(!(s = strtok(argv[2], delimiter)) || !(k = strtok(NULL, delimiter))) {
            printf("Error in arguments");
            freeSections(dictionary, sCount);
            return ERROR_IN_ARGUMENTS;
        }
        
        char* res;
        
        switch(findValue(dictionary, sCount, s, k, &res)) {
            case SECTION_NOT_FOUND:
                printf("Failed to find section [%s]", s);
                freeSections(dictionary, sCount);
                return SECTION_NOT_FOUND;
            case KEY_NOT_FOUND:
                printf("Failed to find key \"%s\" in section [%s]", k, s);
                freeSections(dictionary, sCount);
                return KEY_NOT_FOUND;
            default:
                printf("RESULT: %s\n", res);
                freeSections(dictionary, sCount);
                return 0;
        }

    } else if (!strcmp(argv[2], "expression")) {
        char* arg1;
        const char* op;
        char* arg2;
        const char* space = " ";

        if(!(arg1 = strtok(argv[3], space)) || !(op = strtok(NULL, space)) || !(arg2 = strtok(NULL, space))) {
            printf("Error in arguments!\n");
            freeSections(dictionary, sCount);
            return ERROR_IN_ARGUMENTS;
        }
        char o = op[0];

        const char* s;
        const char* k;
        char* val1;
        char* val2;
        if(!(s = strtok(arg1, delimiter)) || !(k = strtok(NULL, delimiter))) {
            printf("Error in arguments");
            freeSections(dictionary, sCount);
            return ERROR_IN_ARGUMENTS;
        }
        switch(findValue(dictionary, sCount, s, k, &val1)) {
            case SECTION_NOT_FOUND:
                printf("Failed to find section [%s]", s);
                freeSections(dictionary, sCount);
                return SECTION_NOT_FOUND;
            case KEY_NOT_FOUND:
                printf("Failed to find key \"%s\" in section [%s]", k, s);
                freeSections(dictionary, sCount);
                return KEY_NOT_FOUND;
            default:
                break;
        }
        if(!(s = strtok(arg2, delimiter)) || !(k = strtok(NULL, delimiter))) {
            printf("Error in arguments");
            freeSections(dictionary, sCount);
            return ERROR_IN_ARGUMENTS;
        }
        switch(findValue(dictionary, sCount, s, k, &val2)) {
            case SECTION_NOT_FOUND:
                printf("Failed to find section [%s]", s);
                freeSections(dictionary, sCount);
                return SECTION_NOT_FOUND;
            case KEY_NOT_FOUND:
                printf("Failed to find key \"%s\" in section [%s]", k, s);
                freeSections(dictionary, sCount);
                return KEY_NOT_FOUND;
            default:
                break;
        }
        
        if(isNumber(val1) && isNumber(val2)) {
            int a = atoi(val1);
            if(val1[0] == '-') {
                a *= -1;
            }
            int b = atoi(val2);
            if(val2[0] == '-') {
                b *= -1;
            }
            switch (o)
            {
            case '+':
                printf("%d\n", a+b);
                break;
            case '-':
                printf("%d\n", a-b);
                break;
            case '*':
                printf("%d\n", a*b);
                break;
            case '/':
                float A = a;
                if(b !=0) {
                    printf("%f\n", A/b);
                } else {
                    printf("Cannot divide by 0");
                }
                break;
            default:
                printf("Unsupported operator for type INT %c\n", o);
                break;
            }
        } else if(!isNumber(val1) && !isNumber(val2)) {
            if(o == '+') {
                printf("%s", val1);
                printf("%s\n", val2);
            } else {
                printf("Unsupported operator for type STRING - %c\n", o);
            }
        } else {
            printf("[ERROR] Supported opperations <[string] + [string]> and <[int] +,-,/,* [int]>");
        }

    }

    freeSections(dictionary, sCount);

    return 0;
}