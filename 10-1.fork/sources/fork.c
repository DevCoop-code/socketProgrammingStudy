#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int gval = 10;
int main(int argc, char* argv[]) {
    pid_t pid;
    int lval = 20;
    int* intArray = (int*)malloc(sizeof(int) * 3);
    gval++, lval+=5;

    intArray[0] = 1;
    intArray[1] = 2;
    intArray[2] = 3;

    pid = fork();
    if (pid == 0) {     // If child process
        gval += 2, lval += 2, intArray[0] *= 1, intArray[1] *= 2, intArray[2] *= 3;
    } else {            // If Parent process
        gval -= 2, lval -= 2;
    }

    if (pid == 0) {
        printf("Child Proc: [%d %d] [%d %d %d] \n", gval, lval, intArray[0], intArray[1], intArray[2]);
        printf("Heap Memory Address: [%d] \n", intArray);
        free(intArray);
    } else {
        printf("Parent Proc: [%d %d] [%d %d %d] \n", gval, lval, intArray[0], intArray[1], intArray[2]);
        printf("Heap Memory Address: [%d] \n", intArray);
        free(intArray);
    }

    // hankyo: child와 parent의 heap memory 주소가 같게 나오는데 실제 memory 주소는 다르지만 논리 메모리 주소는 process마다 똑같이 인식하는듯 하다

    
    return 0;
}