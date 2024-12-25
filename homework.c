#include <stdio.h>
#include <stdlib.h>


struct employeeNode {
    char record;
    struct employeeNode *nextNode;
};

typedef struct employeeNode employeeNode; 


int main()
{
    char record_1;
    char record_2;

    employeeNode* employee_1 = malloc(sizeof(employeeNode));
    employeeNode* employee_2 = malloc(sizeof(employeeNode));

    printf("Enter a record for employee 1:\n");
    scanf("%c", &record_1);

    printf("Enter a record for employee 2:\n");
    scanf(" %c", &record_2);

    if (employee_1 != NULL)
    {
        employee_1->record = record_1;
        employee_1->nextNode = NULL;
    }

    if (employee_2 != NULL)
    {
        employee_2->record = record_2;
        employee_2->nextNode = NULL;
    }

    employee_1->nextNode = employee_2;

    printf("Employee 1 record: %c\n", employee_1->record);
    printf("Next employee: %p\n", employee_1->nextNode);
    printf("The address of employee in heap: %p\n", employee_1);
    printf("The address of employee in stack: %p\n", &employee_1);

    printf("\nEmployee 2 record: %c\n", employee_2->record);
    printf("Next employee: %p\n", employee_2->nextNode);
    printf("The address of employee in heap: %p\n", employee_2);
    printf("The address of employee in stack: %p\n", &employee_2);

    free(employee_1);
    free(employee_2);

    return 0;
}