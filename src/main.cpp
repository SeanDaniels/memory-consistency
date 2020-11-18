#include "main.h"

#define MODEL 2
#define INPUT 1
#define DBG

using std::cout;

using namespace std;

std::string inputFileDir = "../trace_files/";
std::string spaceString = " ";
std::map<std::string, modelValue> modelSwitch;
/* Contains clock info for single instruction */
std::vector<int> cycleVector (3, 0);
/*Contains clock info for all instructions */
std::vector<vector<int>> cycleVectorList;
/*Contains cache info  */
std::vector<char> cache;

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
    std::vector<std::string> instructionVector;
    std::vector<int> lastInstruction;
    std::string model = argv[MODEL];
    instructionVector = parse_file(argv[INPUT]);
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
    print_cache(cache);
    lastInstruction = cycleVectorList.back();
    cout << "The " << model << " total latency is : " << lastInstruction.at(2) << " cycles" << endl;
    return 0;
}

void wo_simulator(std::vector<std::string> argumentVector){
  static int clockCycle = 0;
  int fetchCycle, issueCycle, retireCycle, cycleTime;
  std::vector<std::string> cacheVector;
  std::vector<string>::iterator it = argumentVector.begin();
  while(++it!=argumentVector.end()){
    print_instruction(*it);
    cycleTime = check_cache(*it);
    cycleVector.at(0) = clockCycle;
    cycleVector.at(1) = clockCycle;
    cycleVector.at(2) = clockCycle + cycleTime;
    print_cycle_vector(cycleVector);
    cycleVectorList.push_back(cycleVector);
    clockCycle++;
  }
  return;
}

void sc_simulator(std::vector<std::string> argumentVector){
  static int clockCycle = 0;
  int fetchCycle, issueCycle, retireCycle, cycleTime;
  std::vector<std::string> cacheVector;
  std::vector<string>::iterator it = argumentVector.begin();
  std::vector<int> checkCycle;
  while(it!=argumentVector.end()){
    print_instruction(*it);
    cycleTime = check_cache(*it);
    cycleVector.at(0) = clockCycle;
    if(cycleVectorList.size() > 0){
      checkCycle = cycleVectorList.back();
      cycleVector.at(1) = checkCycle.at(2);
    }
    else {
      cycleVector.at(1) = clockCycle;
    }
    cycleVector.at(2) = cycleVector.at(1) + cycleTime;
    print_cycle_vector(cycleVector);
    cycleVectorList.push_back(cycleVector);
    clockCycle++;
    it++;
  }
  return;

}

/*Simulator for processor consistency model*/
void pc_simulator(std::vector<std::string> argumentVector){
  static int clockCycle = 0;
  int fetchCycle, issueCycle, retireCycle, cycleTime;
  std::vector<std::string> cacheVector;
  std::vector<string>::iterator it = argumentVector.begin();
  std::vector<int> checkCycle;
  while(it!=argumentVector.end()){
    print_instruction(*it);
    cycleTime = check_cache(*it);
    cycleVector.at(0) = clockCycle;
    if (cycleVectorList.size() > 0) {
      if (*it->begin() == 'L') {
        if (*(it - 1)->begin() == 'S') {
#ifdef DBG
          cout << " (found store/load order)";
#endif
          cycleVector.at(1) = cycleVectorList.back().at(1) + 1;
        }
        else {
          checkCycle = cycleVectorList.back();
          cycleVector.at(1) = checkCycle.at(2);
        }
      }
      else{
        cycleVector.at(1) = cycleVectorList.back().at(2);
      }
    }
    else {
        cycleVector.at(1) = clockCycle;
    }
    cycleVector.at(2) = cycleVector.at(1) + cycleTime;
    print_cycle_vector(cycleVector);
    cycleVectorList.push_back(cycleVector);
    clockCycle++;
    it++;
  }
  return;


}

void print_instruction(std::string instructionString){
  char instruction = instructionString[0];
  char memoryLocation = instructionString[1];
  #ifdef DBG
  cout << "*******************now processing " << instruction << " " << memoryLocation << endl;
  #else
  cout << "*******************now processing " << memoryLocation << endl;
  #endif
  return;
}

void print_cycle_vector(std::vector<int> cycleVector){
  std::vector<int>::iterator it = cycleVector.begin();
  cout << "   { " << *it++;
  for(;it!=cycleVector.end();++it){
    cout << ", " <<*it;
  }
  cout << "}" << endl;
}

int check_cache(string thisMemoryLocation){
 int cycleTime = 100;
 std::vector<char>::iterator it = find(cache.begin(), cache.end(), thisMemoryLocation[1]);
 if(it != cache.end()){
   cout << "We found a hit for " << thisMemoryLocation[1];
   cycleTime = 10;
 }
 else{
   cout << "We found a miss for " << thisMemoryLocation[1];
   cache.push_back(thisMemoryLocation[1]);
 }
 return cycleTime;
}

void print_cache(std::vector<char> thisCache){
  cout << "Size of L1cache is " << thisCache.size() << endl;
}
