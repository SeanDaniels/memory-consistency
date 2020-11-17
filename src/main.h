#ifndef __MAIN_H_
#define __MAIN_H_
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <map>
/*Enumeration of model argument*/
enum modelValue { SC, PC, WO, RC};

void init_map();
/*Parse file*/
std::vector<std::string> parse_file(std::string fileName);
/*Simulator for weak ordering model*/
void wo_simulator(std::vector<std::string> argumentVector);
/*Simulator for processor consistency model*/
void pc_simulator(std::vector<std::string> argumentVector);
/*Simulator for sequential consistency model*/
void sc_simulator(std::vector<std::string> argumentVector);
#endif // __MAIN_H_
