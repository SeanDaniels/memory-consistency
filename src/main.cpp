#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include "main.h"

#define MODEL 0
#define INPUT 1
using namespace std;

int main(int argc, char *argv[]) {
    char* inputFileName = argv[INPUT];
    printf("%s\n", inputFileName);
    return 0;
}
