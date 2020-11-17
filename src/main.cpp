#include "main.h"

#define MODEL 0
#define INPUT 1

using namespace std;

std::string inputFileDir = "../trace_files/";
std::string spaceString = " ";
std::map<std::string, modelValue> modelSwitch;
std::vector<int> cycleVector (3, 0);
std::vector<vector<int>> cycleVectorList;
std::vector<char> cache;

/* Parse file */
std::vector<string> parse_file(std::string fileName){
  std::vector<string> parsed_file_inputs;
  std::string inputFile = inputFileDir + fileName;
  std::string fileLine, arg, firstLetter, lastLetter;
  ifstream infile(inputFile);
  size_t foundSpace, len;

  cout << "Opening file " << inputFile << endl;
  cout << "Parsing file " << inputFile << endl;

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
    instructionVector = parse_file(argv[INPUT]);
    wo_simulator(instructionVector);
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

void print_instruction(std::string instructionString){
  char instruction = instructionString[0];
  char memoryLocation = instructionString[1];
  std::cout << "***Instruction: " << endl;
  if (instruction == 'L') {
    std::cout << "Load ";
  } else if (instruction == 'S') {
    std::cout << "Store ";
  } else if (instruction == 'X') {
    std::cout << "Lock ";
  } else if (instruction == 'U') {
    std::cout << "Unlock ";
  } else {
    std::cout << "Error" << endl;
  }
  std::cout << memoryLocation << endl;
  return;
}

void print_cycle_vector(std::vector<int> cycleVector){
  std::vector<int>::iterator it = cycleVector.begin();
  cout << "{ " << *it++;
  for(;it!=cycleVector.end();++it){
    cout << ", " <<*it;
  }
  cout << " }" << endl;
}

int check_cache(string thisMemoryLocation){
 int cycleTime = 100;
 std::vector<char>::iterator it = find(cache.begin(), cache.end(), thisMemoryLocation[1]);
 if(it != cache.end()){
   cout << "Hit" << endl;
   cycleTime = 10;
 }
 else{
   cout << "Miss" << endl;
   cache.push_back(thisMemoryLocation[1]);
 }
 return cycleTime;
}
