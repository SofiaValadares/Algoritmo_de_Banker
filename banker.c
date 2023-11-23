#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CUSTOMER_FILE "customer.txt"
#define COMMANDS_FILE "commands.txt"
#define RESULT_FILE "result.txt"


int NUMBER_OF_RESOURCES;
int NUMBER_OF_CUSTOMER;

typedef struct customers {
    int maximum;
    int allocation;
    int need;
} Customers;


void get_number_of_resources(int argc);
void get_number_of_customers();
int *get_resources(char *argv[]);
int *get_request();
Customers **get_customers();
int check_RQ(int *resquest, Customers **customers, int customer_number, int *available);
void exec_RQ(Customers ***customers, int **available, int *request, int customer_number);
int check_RL(int *resquest, Customers *customers, int customer_number);
void exec_RL(Customers ***customers, int **available, int *request, int customer_number);
void print_status(Customers **customers, int *available);

int main(int argc, char *argv[]) {
    get_number_of_resources(argc);
    get_number_of_customers();
    
    FILE *original_stdout = stdout;
    FILE *original_stdin = stdin;

    int *available = get_resources(argv);

    Customers **customers = get_customers();

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
                if (check_RQ(request, customers, customer_number, available)) {
                    exec_RQ(&customers, &available, request, customer_number);
                }
            } else if (strcmp(command, "RL") == 0) {
                if (check_RL(request, customers[customer_number], customer_number)) {
                    exec_RL(&customers, &available, request, customer_number);
                }
            }

            free(request);
        } else if (strcmp(command, "*") == 0) {
            print_status(customers, available);
        }
    }   

    fflush(stdout);
    fflush(stdin);

    dup2(fileno(original_stdout), fileno(stdout));
    dup2(fileno(original_stdin), fileno(stdout));

    free(available);
    free(customers);

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

    char line[1501];

    while (fgets(line, sizeof(line), f) != NULL) {
        int tan = -1;

        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == ' ') {
                tan++;
            }
        }      

        if (tan != NUMBER_OF_RESOURCES && line[0] != '*') {
            fprintf(stderr, "Incompatibility between commands.txt and command line\n");
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

    char line[1501];

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

int *get_resources(char *argv[]) {
    int *resources = (int*)malloc(NUMBER_OF_RESOURCES* sizeof(int));

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        resources[i] = atoi(argv[i + 1]);
    }

    return resources;
}

int *get_resources_customers(char line[]) {
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

Customers **get_customers_alocation() {
    Customers **customers = (Customers **)malloc(NUMBER_OF_CUSTOMER * sizeof(Customers *));

    for (int i = 0; i < NUMBER_OF_CUSTOMER; i++) {
        customers[i] = (Customers *)malloc(NUMBER_OF_RESOURCES *sizeof(Customers));

        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            customers[i][j].maximum = 0;
            customers[i][j].allocation = 0;
            customers[i][j].need = 0;
        }
    }

    return customers;
}

Customers **get_customers() {
    Customers **customers = get_customers_alocation();

    FILE *f = NULL;

    f = fopen(CUSTOMER_FILE, "r");

    if (f == NULL) {
        fprintf(stderr, "Fail to read customer.txt\n");
        exit(0);
    }

    for (int i = 0; i < NUMBER_OF_CUSTOMER; i++) {
        char line[1501];

        fgets(line, sizeof(line), f);

        int *resources_customers = get_resources_customers(line);

        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            customers[i][j].maximum = resources_customers[j];
            customers[i][j].need = customers[i][j].maximum - customers[i][j].allocation;
        }
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

int check_safe_state(int *resquest, Customers **customers, int customer_number, int *available) {
    Customers **customers_copy = get_customers_alocation();
    int *available_copy = malloc(NUMBER_OF_RESOURCES * sizeof(int));

    for (int i = 0; i < NUMBER_OF_CUSTOMER; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            customers_copy[i][j].maximum = customers[i][j].maximum;
            customers_copy[i][j].allocation = customers[i][j].allocation;
            customers_copy[i][j].need = customers[i][j].need;
        }
    }
    
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        customers_copy[customer_number][i].allocation += resquest[i];
        customers_copy[customer_number][i].need -= resquest[i];

        available_copy[i] = available[i] - resquest[i];
    }
    
    int *finish = (int*)malloc(NUMBER_OF_CUSTOMER * sizeof(int));
 
    for (int i = 0; i < NUMBER_OF_CUSTOMER; i++) {
        finish[i] = 0;
    }
 
    int *work = (int*)malloc(NUMBER_OF_RESOURCES * sizeof(int));
    
    for (int i = 0; i < NUMBER_OF_RESOURCES ; i++) {
        work[i] = available_copy[i];
    }
        
    int count = 0;
    while (count < NUMBER_OF_CUSTOMER)
    {
        int safe = 0;
        for (int i = 0; i < NUMBER_OF_CUSTOMER; i++)
        {
            if (finish[i] == 0)
            {
                int can_allocate = 1;

                for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                    if (customers_copy[i][j].need > work[j]) {
                        can_allocate = 0;
                        break;
                    }
                }

                if (can_allocate)
                {
                    for (int k = 0 ; k < NUMBER_OF_RESOURCES ; k++) {
                        work[k] += customers_copy[i][k].allocation;
                    }

                    count++;

                    finish[i] = 1;
                    safe = 1;
                }
            }
        }
 
        if (!safe)
        {
            free(customers_copy);
            free(available_copy);
            free(finish);
            free(work);
            return 1;
        }
    }
    
    free(customers_copy);
    free(available_copy);
    free(finish);
    free(work);

    return 0;
}

int check_RQ(int *resquest, Customers **customers, int customer_number, int *available) {
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (resquest[i] > customers[customer_number][i].need || resquest[i] > customers[customer_number][i].maximum - customers[customer_number][i].allocation) {
            printf("The customer %d request ", customer_number);

            for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                printf("%d ", resquest[i]);
            }

            printf("was denied because exceed its maximum need\n");   

            return 0;
        }

        if (resquest[i] > available[i]) {
            printf("The resources ");
 
            for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                printf("%d ", available[j]);
            }

            printf("are not enough to customer %d request ", customer_number);

            for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                printf("%d ", resquest[j]);
            }

            printf("\n");

            return 0;
        }

        if (check_safe_state(resquest, customers, customer_number, available)) {
            printf("The customer %d request ", customer_number);

            for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                printf("%d ", resquest[j]);
            }

            printf("was denied because result in an unsafe state\n");
            return 0;
        }
    }

    return 1;
}

void exec_RQ(Customers ***customers, int **available, int *request, int customer_number) {
    printf("Allocate to customer %d the resources ", customer_number);

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        (*customers)[customer_number][i].allocation += request[i];
        (*customers)[customer_number][i].need = (*customers)[customer_number][i].maximum - (*customers)[customer_number][i].allocation;

        (*available)[i] -= request[i];

        printf("%d ", request[i]);
    }

    printf("\n");
}

int check_RL(int *resquest, Customers *customers, int customer_number) {
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (resquest[i] > customers[i].allocation) {
            printf("The customer %d released ", customer_number);

            for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                printf("%d ", resquest[i]);
            }

            printf("was denied because exceed its maximum allocation\n");   

            return 0;
        }
    }

    return 1;
}

void exec_RL(Customers ***customers, int **available, int *request, int customer_number) {
    printf("Release from customer %d the resources ", customer_number);

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        (*customers)[customer_number][i].allocation -= request[i];
        (*customers)[customer_number][i].need = (*customers)[customer_number][i].maximum - (*customers)[customer_number][i].allocation;

        (*available)[i] += request[i];

        printf("%d ", request[i]);
    }

    printf("\n");
}

Customers *get_numbers_formats(Customers **customers) {
    Customers *numbers_formats = (Customers*)malloc(NUMBER_OF_RESOURCES * sizeof(Customers));

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        Customers biggest_numbers = {1, 1, 1};

        for (int j = 0; j < NUMBER_OF_CUSTOMER; j++) {
            if (customers[j][i].maximum > biggest_numbers.maximum) {
                biggest_numbers.maximum = customers[j][i].maximum;
            }

            if (customers[j][i].allocation > biggest_numbers.allocation) {
                biggest_numbers.allocation = customers[j][i].allocation;
            }

            if (customers[j][i].need > biggest_numbers.need) {
                biggest_numbers.need = customers[j][i].need;
            }
        }

        numbers_formats[i].maximum = 0;

        while (biggest_numbers.maximum > 0) {
            biggest_numbers.maximum /= 10;
            numbers_formats[i].maximum++;
        }

        numbers_formats[i].allocation = 0;

        while (biggest_numbers.allocation > 0) {
            biggest_numbers.allocation /= 10;
            numbers_formats[i].allocation++;
        }

        numbers_formats[i].need= 0;

        while (biggest_numbers.need> 0) {
            biggest_numbers.need /= 10;
            numbers_formats[i].need++;
        }
        
    }

    return numbers_formats;
}

Customers get_size_line_numbers(Customers *numbers_formats) {
    Customers size_line_numbers = {0, 0, 0};

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        size_line_numbers.maximum += numbers_formats[i].maximum;
        size_line_numbers.maximum++;

        size_line_numbers.allocation += numbers_formats[i].allocation;
        size_line_numbers.allocation++;

        size_line_numbers.need += numbers_formats[i].need;
        size_line_numbers.need++;
    }

    return size_line_numbers;
}

void print_status(Customers **customers, int *available) {
    Customers *numbers_formats = get_numbers_formats(customers);
    Customers size_line_numbers = get_size_line_numbers(numbers_formats);

    printf("MAXIMUM ");

    for (int i = 8; i < size_line_numbers.maximum; i++) {
        printf(" ");
    }

    printf("| ALLOCATION ");

    for (int i = 11; i < size_line_numbers.allocation; i++) {
        printf(" ");
    }

    printf("| NEED\n");

    for (int i = 0; i < NUMBER_OF_CUSTOMER; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            printf("%*d ", numbers_formats[j].maximum, customers[i][j].maximum);
        }

        for (int j = size_line_numbers.maximum; j < 8; j++) {
            printf(" ");
        }

        printf("| ");

        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            printf("%*d ", numbers_formats[j].allocation, customers[i][j].allocation);
        }

        for (int j = size_line_numbers.allocation; j < 11; j++) {
            printf(" ");
        }

        printf("| ");

        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            printf("%*d ", numbers_formats[j].need, customers[i][j].need);
        }

        printf("\n");
    }

    printf("AVAILABLE ");

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        printf("%d ", available[i]);
    }

    printf("\n");
}
