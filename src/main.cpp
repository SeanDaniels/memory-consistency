#include "main.h"

#define MODEL 2
#define INPUT 1

using std::cout;

using namespace std;

std::string inputFileDir = "../trace_files/";
std::string spaceString = " ";
std::map<std::string, modelValue> modelSwitch;
/* Contains clock info for instructions */
std::vector<int> cycleVector (3, 0);
/*  */
std::vector<vector<int>> cycleVectorList;
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
    instructionVector = parse_file(argv[INPUT]);
    sc_simulator(instructionVector);
    print_cache(cache);
    lastInstruction = cycleVectorList.back();
    cout << "The " << argv[MODEL] << " total latency is : " << lastInstruction.at(2) << " cycles" << endl;
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

void print_instruction(std::string instructionString){
  char instruction = instructionString[0];
  char memoryLocation = instructionString[1];
  cout << "*******************now processing " << memoryLocation << endl;
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
