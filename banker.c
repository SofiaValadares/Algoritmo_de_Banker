#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CUSTOMER_FILE "customer.txt"
#define COMMANDS_FILE "commands.txt"
#define RESULT_FILE "result.txt"

int NUMBER_OF_RESOURCES;
int NUMBER_OF_CUSTOMER;

void get_number_of_resources(int argc);
void get_number_of_customers();
const int *get_resources(char *argv[]);
int *get_resources_available(const int *resources, int **customers_alloction);
int **get_customers_alocation();
const int **get_customers();
int *get_request();
int check_RQ(int *resquest, const int *customers_maximum, int *customers_alloction, int customer, int *availables_resources);
void exec_RQ(int ***customers_alloction, int *request, int customer);
int check_RL(int *resquest, int *customers_alloction, int customer);
void exec_RL(int ***customers_alloction, int *request, int customer);

int main(int argc, char *argv[]) {
    get_number_of_resources(argc);
    get_number_of_customers();
    
    const int *resources = get_resources(argv);
    const int **customers_maximum = get_customers();
    int **customers_alloction = get_customers_alocation();


    FILE *commands = NULL;
    commands = fopen(COMMANDS_FILE, "r");

    if (commands == NULL) {
        fprintf(stderr, "Fail to read commands.txt\n");
        exit(0);
    }

    FILE *original_stdout = stdout;
    FILE *original_stdin = stdin;

    freopen(RESULT_FILE, "w", stdout);
    freopen(COMMANDS_FILE, "r", stdin);

    while (1) {
        char command[3];

        if (scanf("%s", command) == EOF) {
            break;
        }

        if (strcmp(command, "RQ") == 0 || strcmp(command, "RL") == 0) {
            int customer_number;

            scanf("%d", &customer_number);

            int *request = get_request();

            if (strcmp(command, "RQ") == 0) {
                int *availables_resources = get_resources_available(resources, customers_alloction);

                if (check_RQ(request, customers_maximum[customer_number], customers_alloction[customer_number], customer_number, availables_resources)) {
                    exec_RQ(&customers_alloction, request, customer_number);
                }

            } else if (strcmp(command, "RL") == 0) {

                if (check_RL(request, customers_alloction[customer_number], customer_number)) {
                    exec_RL(&customers_alloction, request, customer_number);
                }
            }
        } else if (strcmp(command, "*") == 0) {
            
        }


    }   

    /*for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        printf("%d ", resources[i]);
    }
    printf("\n");

    for (int i = 0; i < NUMBER_OF_CUSTOMER; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            printf("%d ", customers[i][j]);
        }
        printf("\n");
    }*/

    fflush(stdout);
    fflush(stdin);

    dup2(fileno(original_stdout), fileno(stdout));
    dup2(fileno(original_stdin), fileno(stdout));

    fclose(commands);

    return 0;
}

void check_commands_file() {
    FILE *f = NULL;

    f = fopen(COMMANDS_FILE, "r");

    if (f == NULL) {
        fprintf(stderr, "Fail to read commands.txt\n");
        exit(0);
    }

    fclose(f);
}

void check_commands_file2() {
    FILE *f = NULL;

    f = fopen(COMMANDS_FILE, "r");

    if (f == NULL) {
        fprintf(stderr, "Fail to read commands.txt\n");
        exit(0);
    }

    char line[101];

    while (fgets(line, sizeof(line), f) != NULL) {
        int tan = -1;

        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == ' ') {
                tan++;
            }
        }      

        if (tan != NUMBER_OF_RESOURCES && line[0] != '*') {
            fprintf(stderr, "Incompatibility between customer.txt and command line\n");
            exit(0);
        }  
    }

    fclose(f);
}

void get_number_of_resources(int argc) {
    if (NUMBER_OF_RESOURCES == 0) {
        NUMBER_OF_RESOURCES = argc - 1;
    }
}

void get_number_of_customers() {
    check_commands_file();

    if (NUMBER_OF_CUSTOMER != 0) {
        return;
    }

    FILE *f = NULL;

    f = fopen(CUSTOMER_FILE, "r");

    if (f == NULL) {
        fprintf(stderr, "Fail to read customer.txt\n");
        exit(0);
    }

    char line[101];

    while (fgets(line, sizeof(line), f) != NULL) {
        NUMBER_OF_CUSTOMER++;

        int tan = 1;

        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == ',') {
                tan++;
            }
        }      

        if (tan != NUMBER_OF_RESOURCES) {
            fprintf(stderr, "Incompatibility between customer.txt and command line\n");
            exit(0);
        }  
    }

    fclose(f);

    check_commands_file2();
}

const int *get_resources(char *argv[]) {
    int *resources = (int*)malloc(NUMBER_OF_RESOURCES* sizeof(int));

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        resources[i] = atoi(argv[i + 1]);
    }

    return resources;
}

int *resources_customers(char line[]) {
    int *resources = (int*)malloc(NUMBER_OF_RESOURCES * sizeof(int));

    int ind = 0;

    char *token = strtok(line, ",");

    while (token != NULL) {
        resources[ind] = atoi(token);

        token = strtok(NULL, ",");
        ind++;
    }

    return resources;
}

int **get_customers_alocation() {
    int **customers = (int**)malloc(NUMBER_OF_CUSTOMER * sizeof(int*));

    for (int i = 0; i < NUMBER_OF_CUSTOMER; i++) {
        customers[i] = (int*)malloc(NUMBER_OF_RESOURCES *sizeof(int));

        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            customers[i][j] = 0;
        }
    }

    return customers;
}

const int **get_customers() {
    const int **customers = (const int**) get_customers_alocation();

    FILE *f = NULL;

    f = fopen(CUSTOMER_FILE, "r");

    if (f == NULL) {
        fprintf(stderr, "Fail to read customer.txt\n");
        exit(0);
    }

    for (int i = 0; i < NUMBER_OF_CUSTOMER; i++) {
        char line[101];

        fgets(line, sizeof(line), f);

        customers[i] = resources_customers(line);
    }

    fclose(f);

    return customers;
}

int *get_request() {
    int *request = (int*)malloc(NUMBER_OF_RESOURCES * sizeof(int));

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        scanf("%d", &request[i]);
    }

    return request;
}

int *get_customer_need(const int *customers_maximum, int *customers_alloction) {
    int *customer_need = (int*)malloc(NUMBER_OF_RESOURCES * sizeof(int));

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        customer_need[i] = customers_maximum[i] - customers_alloction[i];
    }

    return customer_need;
}

int *get_resources_available(const int *resources, int **customers_alloction) {
    int *available = (int*)malloc(NUMBER_OF_RESOURCES * sizeof(int));

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        int sun_used_resurces = 0;

        for (int j = 0; j < NUMBER_OF_CUSTOMER; j++) {
            sun_used_resurces += customers_alloction[j][i];
        }

        available[i] = resources[i] - sun_used_resurces;
    }

    return available;
}

int check_RQ(int *resquest, const int *customers_maximum, int *customers_alloction, int customer, int *availables_resources) {
    int *customer_need = get_customer_need(customers_maximum, customers_alloction);
    int check = 1;

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (customer_need[i] < resquest[i]) {
            check = 0;
            break;
        }

        if (availables_resources[i] < resquest[i]) {
            printf("The resources ");

            for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                printf("%d ", availables_resources[i]);
            }

            printf("are not enough to customer %d request ", customer);

            for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                printf("%d ", resquest[i]);
            }

            printf("\n");

            return 0;
        }
    }

    if (!check) {
        printf("The customer %d request ", customer);

        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            printf("%d ", resquest[i]);
        }

        printf("was denied because exceed its maximum need\n");        
    }

    return check;
}

void exec_RQ(int ***customers_alloction, int *request, int customer) {
    printf("Allocate to customer %d the resources ", customer);

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        (*customers_alloction)[customer][i] += request[i];

        printf("%d ", request[i]);
    }

    printf("\n");
}

int check_RL(int *resquest, int *customers_alloction, int customer) {
    int check = 1;

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (customers_alloction[i] < resquest[i]) {
            check = 0;
            break;
        }
    }

    if (!check) {
        printf("The customer %d released ", customer);

        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            printf("%d ", resquest[i]);
        }

        printf("was denied because exceed its maximum allocation\n");        
    }

    return check;
}

void exec_RL(int ***customers_alloction, int *request, int customer) {
    printf("Release from customer %d the resources ", customer);

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        (*customers_alloction)[customer][i] -= request[i];

        printf("%d ", request[i]);
    }

    printf("\n");
}
