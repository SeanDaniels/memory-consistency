#ifndef __MAIN_H_
#define __MAIN_H_
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <map>
#include <utility>
#include <algorithm>
#include <array>
#include "print.h"

typedef struct {
    std::vector<int> cycle_info;
    char operation;
    char memoryLocation;
}instruction;

typedef std::vector<char>::iterator cache_iterator;

typedef std::vector<instruction>::iterator instruction_iterator;

/*Parse file*/
std::vector<std::string> parse_file(std::string fileName);
/*Simulator for release consistency model*/
void rc_simulator(std::vector<std::string> argumentVector);
/*Simulator for weak ordering model*/
void wo_simulator(std::vector<std::string> argumentVector);
/*Simulator for processor consistency model*/
void pc_simulator(std::vector<std::string> argumentVector);
/*Simulator for sequential consistency model*/
void sc_simulator(std::vector<std::string> argumentVector);
/*Check cache*/
int check_cache(char thisMemoryLocation);
/*Find max time*/
int find_max_finish();
/*Find max time*/
int find_last_load();
/*Check store buffer*/
void check_store_buffer(char thisMemoryLocation);
/*flush store buffer*/
void flush_store_buffer(int currentClockCycle);
/* update historyy */
void update_instruction_history(int currentClockCycle, int cycleTime, char thisMemoryLocation);
/* locate in history */
instruction_iterator locate_in_instruction_history(char thisMemoryLocation);

int check_for_dependency(char thisMemoryLocation);

#endif // __MAIN_H_
