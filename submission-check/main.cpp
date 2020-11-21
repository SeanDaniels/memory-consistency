#include "main.h"

#define MODEL 2
#define INPUT 1
#define SPACE_STRING " "
#define DBG

using std::cout;
using std::string;
using std::ifstream;
using std::vector;
using std::endl;
//using namespace std;

std::string inputFileDir = "./";
std::string spaceString = " ";

/* Contains clock info for single instruction */
std::vector<int> cycleVector (3, 0);

/*Contains clock info for all instructions */
std::vector<vector<int>> cycleVectorList;

/*Contains cache info  */
std::vector<char> cacheVector;

/*Contains store buffer info */
std::vector<char> storeBuffer;

/* Parse file */
std::vector<string> parse_file(std::string fileName){
  std::vector<string> parsed_file_inputs;
  std::string inputFile = inputFileDir + fileName;
  std::string fileLine, arg, firstLetter, lastLetter;
  ifstream infile(inputFile);
  size_t foundSpace, len;

  while (getline(infile, fileLine)) {
    foundSpace = fileLine.find(spaceString);
    firstLetter = fileLine.front();
    lastLetter = fileLine.back();
    if (foundSpace == string::npos) {
      if (firstLetter == "L") {
        firstLetter = "X";
      }
    }
    arg = firstLetter + lastLetter;
    parsed_file_inputs.push_back(arg);
    }
    return parsed_file_inputs;
}

/* Main */
int main(int argc, char *argv[]) {
    std::vector<std::string> instructionVector = parse_file(argv[INPUT]);
    std::vector<int> lastInstruction;
    std::string model = argv[MODEL];
    if(model == "SC"){
      sc_simulator(instructionVector);
    }
    else if (model == "PC"){
     pc_simulator(instructionVector);
    }
    else if (model == "WO"){
     return 0;
    }
    else if (model == "RC"){
     return 0;
    }
    else{
      cout << "ERROR: model type not valid" << endl;
      return 0;
    }

    #ifdef DBG
    print_cache(cacheVector);
    cout << "The " << model << " total latency is : " << lastInstruction.at(2) << " cycles" << endl;
    #else
    #endif
    return 0;
}


void wo_simulator(std::vector<std::string> argumentVector){
  static int clockCycle = 0;
  std::vector<string>::iterator it = argumentVector.begin();
  std::vector<int> cycleInfo;
  char previousInstruction, currentInstruction, location;
  int  avgCSTime, lockIssue, unlockComplete, csTime, csNumber, cycleTime, previousLoad, checkBack = 0;
  instruction_iterator i_it;
  csTime = 0;
  csNumber = 0;
  while(it!=argumentVector.end()){
    #ifdef DBG
    print_instruction(*it);
    #endif
    currentInstruction = it->at(0);
    location = it->at(1);
    instructionHistory.push_back(instruction());
    instructionHistory.back().memoryLocation = location;
    instructionHistory.back().operation = currentInstruction;
    //if memory location exists in cache, cycle time is 10, if not, cycle time is 100
    cycleTime = check_cache(location);
    //set cycle time
    cycleVector.at(0) = clockCycle;
    // if this isn't the first instruction
    if(instructionHistory.size()==1){
      cycleVector.at(1) = clockCycle;
    }
    else{
      //PROCESS LOCK
      if(currentInstruction == 'X'){
       cycleVector.at(1) = cycleVectorList.back().at(2);
       lockIssue = cycleVector.at(1);
       csNumber++;
      }
      //PROCESS UNLOCK
      else if (currentInstruction == 'U') {
        flush_store_buffer(clockCycle);
        cycleVector.at(1) = find_max_finish();
        unlockComplete = cycleVector.at(1) + cycleTime;
        csTime+=(unlockComplete - lockIssue);
      }
      else{
        i_it = locate_in_instruction_history(location);
        if(i_it!=instructionHistory.end()){
          cycleVector.at(1) = i_it->cycle_info.at(2);
        }
        else{
          cycleVector.at(1) = cycleVectorList.back().at(2);
        }
      }
    }
    //update retire time
    cycleVector.at(2) = cycleVector.at(1) + cycleTime;
    //add cycle info to instruction history
    instructionHistory.back().cycle_info = cycleVector;
    //unlock routine
    #ifdef DBG
    if(cycleTime==100)
      cout << "We got a miss for " << location;
    else
      cout << "We got a hit for " << location;
    print_cycle_vector(cycleVector);
    #endif
    cycleVectorList.push_back(cycleVector);
    clockCycle++;
    it++;
  }
  cout<< instructionHistory.back().cycle_info.at(2) << " "<< csTime/csNumber << endl;
  return;
}

/*Simulator for sequential consistency model*/
void sc_simulator(std::vector<std::string> argumentVector){
  static int clockCycle = 0;
  std::vector<string>::iterator it = argumentVector.begin();
  std::vector<int> cycleInfo;
  char currentInstruction, location;
  int  lockIssue, unlockComplete, csTime = 0, csNumber = 0, cycleTime = 0;
  while (it != argumentVector.end()) {
#ifdef DBG
    print_instruction(*it);
#endif
    currentInstruction = it->at(0);
    location = it->at(1);
    instructionHistory.push_back(instruction());
    instructionHistory.back().memoryLocation = location;
    instructionHistory.back().operation = currentInstruction;
    cycleTime = check_cache(it->at(1));
    cycleVector.at(0) = clockCycle;
    if(instructionHistory.size() > 1){
      cycleInfo = cycleVectorList.back();
      cycleVector.at(1) = cycleInfo.at(2);
    }
    else {
      cycleVector.at(1) = clockCycle;
    }
    cycleVector.at(2) = cycleVector.at(1) + cycleTime;
    if(currentInstruction=='X'){
      lockIssue = cycleVector.at(1);
      csNumber++;
    }
    if (currentInstruction == 'U') {
      unlockComplete = cycleVector.at(1) + cycleTime;
      csTime += (unlockComplete - lockIssue);
    }
// #ifdef DBG
//     print_cycle_vector(cycleVector);
// #endif
    cycleVectorList.push_back(cycleVector);
    instructionHistory.back().cycle_info = cycleVector;
    clockCycle++;
    it++;
  }
  cout<< instructionHistory.back().cycle_info.at(2) << " "<< csTime/csNumber << endl;
  return;

}

/*Simulator for processor consistency model*/
void pc_simulator(std::vector<std::string> argumentVector){
  static int clockCycle = 0;
  std::vector<string>::iterator it = argumentVector.begin();
  std::vector<int> cycleInfo;
  char previousInstruction, currentInstruction, location;
  int  avgCSTime, lockIssue, unlockComplete, csTime, csNumber, cycleTime, previousLoad, checkBack = 0;
  csTime = 0;
  csNumber = 0;
  while(it!=argumentVector.end()){
    #ifdef DBG
    print_instruction(*it);
    #endif
    currentInstruction = it->at(0);
    location = it->at(1);
    instructionHistory.push_back(instruction());
    instructionHistory.back().memoryLocation = location;
    instructionHistory.back().operation = currentInstruction;
    //if not store check cache for memory location
    //if memory location exists in cache, cycle time is 10, if not, cycle time is 100
    if(*it->begin() != 'S'){
      cycleTime = check_cache(location);
    }
    //if store, check store buffer for memory location
    //cycle time will always be one
    else{
      check_store_buffer(location);
      cycleTime = 1;
    }
    //set cycle time
    cycleVector.at(0) = clockCycle;
    // if this isn't the first instruction
    if (instructionHistory.size() > 1) {
      if(currentInstruction == 'L'){
        previousInstruction = *(it-1)->begin();
        if((previousInstruction=='S')||(previousInstruction == 'U'))
          cycleVector.at(1) = find_last_load();
        else
          cycleVector.at(1) = cycleVectorList.back().at(2);
      }
      else if(currentInstruction == 'X'){
       cycleVector.at(1) = cycleVectorList.back().at(2);
       lockIssue = cycleVector.at(1);
       csNumber++;
      } else if (currentInstruction == 'U') {
        flush_store_buffer(clockCycle);
        cycleVector.at(1) = find_max_finish();
        unlockComplete = cycleVector.at(1) + cycleTime;
        csTime+=(unlockComplete - lockIssue);
      } else
        cycleVector.at(1) = cycleVectorList.back().at(2);
    }
    // this is the first instrution
    else {
      cycleVector.at(1) = clockCycle;
    }
    //update retire time
    cycleVector.at(2) = cycleVector.at(1) + cycleTime;
    //add cycle info to instruction history
    instructionHistory.back().cycle_info = cycleVector;
    //unlock routine
    #ifdef DBG
    if(cycleTime==100)
      cout << "We got a miss for " << location;
    else
      cout << "We got a hit for " << location;
    print_cycle_vector(cycleVector);
    #endif
    cycleVectorList.push_back(cycleVector);
    clockCycle++;
    it++;
  }
  cout<< instructionHistory.back().cycle_info.at(2) << " "<< csTime/csNumber << endl;
  return;
}

void print_instruction(std::string instructionString){
  char instruction = instructionString[0];
  char memoryLocation = instructionString[1];
  #ifdef DBG
  cout << "*******************now processing " << instruction << " " << memoryLocation << endl;
  #endif
  return;
}

int check_cache(char thisMemoryLocation){
 int cycleTime = 100;
 std::vector<char>::iterator it = find(cacheVector.begin(), cacheVector.end(), thisMemoryLocation);
 if(it != cacheVector.end()){
   cycleTime = 10;
 }
 else{
   cacheVector.push_back(thisMemoryLocation);
 }
 return cycleTime;
}

void check_store_buffer(char thisMemoryLocation){
 std::vector<char>::iterator it = find(storeBuffer.begin(), storeBuffer.end(), thisMemoryLocation);
 if(it != storeBuffer.end()){
   #ifdef DBG
   cout << "Edited " << thisMemoryLocation << " in the store buffer as there was a hit ";
   #endif
 }
 else{
   #ifdef DBG
   cout << "Added " << thisMemoryLocation << " to the Store Buffer without any eviction " ;
   #endif
   storeBuffer.push_back(thisMemoryLocation);
 }
 return;
}

void print_cache(std::vector<char> thisCache){
  cache_iterator it = thisCache.begin();
  instruction_iterator i_it;
  cout << endl;
  if(thisCache.size()>0){
    for (; it != thisCache.end(); it++) {
      cout << *it;
      i_it = locate_in_instruction_history(*it);
      if (i_it != instructionHistory.end()) {
//#ifdef DBG
        print_cycle_vector(i_it->cycle_info);
//#endif
      }
    }
  }
  return;
}

void flush_store_buffer(int currentClockCycle){
  cache_iterator it = storeBuffer.begin();
  instruction_iterator i_it;
  instruction_iterator previousCompletion = (instructionHistory.end()) - 2;
  int cycleTime, startCycle = previousCompletion->cycle_info.at(2);//instructionHistory.back().cycle_info.at(2);
#ifdef DBG
  print_cycle_vector((previousCompletion)->cycle_info);
#endif
  it = storeBuffer.begin();
  for(; it!=storeBuffer.end(); it++){
    #ifdef DBG
    cout << "Checking for " << *it << endl;
    #endif
    cycleTime = check_cache(*it);
    #ifdef DBG
    if(cycleTime==100){
      cout << "We got a miss for " << *it;
    }
    else{
      cout << "We got a hit for " << *it;
    }
    #endif
    i_it = locate_in_instruction_history(*it);
    if(i_it!=instructionHistory.end()){
      i_it->cycle_info.at(1) = startCycle;
      i_it->cycle_info.at(2) = startCycle + cycleTime;
      #ifdef DBG
      print_cycle_vector(i_it->cycle_info);
      #endif
    }
    else{
      #ifdef DBG
      cout << "Error, didn't find instruction in instruction history" << endl;
      #endif
    }

  }
  storeBuffer.clear();
  return;
}

void update_instruction_history(int currentClockCycle, int cycleTime, char thisMemoryLocation){
  std::vector<instruction>::iterator instruction_it = instructionHistory.end();
  for(;instruction_it!=instructionHistory.begin()+1;instruction_it--){
    if (instruction_it->memoryLocation == thisMemoryLocation) {
      instruction_it->cycle_info.at(1) = currentClockCycle;
      instruction_it->cycle_info.at(2) = currentClockCycle + cycleTime;
      return;
    }
  }
  cout << endl ;
return;
}

instruction_iterator locate_in_instruction_history(char thisMemoryLocation) {
  instruction_iterator instruction_it = instructionHistory.end();
  for (; instruction_it != instructionHistory.begin() + 1; instruction_it--) {
    if (instruction_it->memoryLocation == thisMemoryLocation) {
      return instruction_it;
    }
  }
  return instructionHistory.end();
}

int find_max_finish(){
  int checkMax = 0, maxFinish = 0;
  instruction_iterator it = instructionHistory.begin();
  for (; it != (instructionHistory.end()-1); it++) {
    if(it->cycle_info.at(2)>maxFinish)
      maxFinish = it->cycle_info.at(2);
  }
  return maxFinish;
}

int find_last_load(){
  int checkMax = 0, maxFinish = 0;
  instruction_iterator it = instructionHistory.begin();
  for (; it != (instructionHistory.end() - 1); it++) {
    if (it->operation == 'L') {
      if (it->cycle_info.at(2) > maxFinish)
        maxFinish = it->cycle_info.at(2);
    }
  }
  return maxFinish;
}

// int check_for_dependency(char thisMemoryLocation){
//   int checkMax = 0, maxFinish = 0;
//   instruction_iterator it = instructionHistory.begin();
//   for (; it != (instructionHistory.end()-1); it++) {
//     if(it->memoryLocation == thisMemoryLocation)
//       maxFinish = it->cycle_info.at(2);
//   }
//   return maxFinish;

// }
