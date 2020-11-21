#include "print.h"

using std::string;
using std::cout;
using std::endl;
using std::vector;

extern vector<instruction> instructionHistory;

void print_instruction(std::string instructionString){
  char instruction = instructionString[0];
  char memoryLocation = instructionString[1];
  cout << "*******************now processing " << instruction << " " << memoryLocation << endl;
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

void print_cache(std::vector<char> thisCache){
  cache_iterator it = thisCache.begin();
  instruction_iterator i_it;
  cout << endl;
  if(thisCache.size()>0){
    for (; it != thisCache.end(); it++) {
      cout << *it;
      i_it = locate_in_instruction_history(*it);
      if(i_it!=instructionHistory.end())
        print_cycle_vector(i_it->cycle_info);
    }
  }
  return;
}
