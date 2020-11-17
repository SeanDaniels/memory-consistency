#include "main.h"

#define MODEL 0
#define INPUT 1

using namespace std;

std::string inputFileDir = "../trace_files/";
std::string spaceString = " ";
std::map<std::string, modelValue> modelSwitch;

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
    for (std::vector<string>::iterator it = instructionVector.begin();
         it != instructionVector.end(); ++it) {
      cout << *it->begin() << endl;

    }
    return 0;
}
