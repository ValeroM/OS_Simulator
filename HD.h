/*
Author: Marco Valero
*/

#ifndef HD_H_
#define HD_H_

#include <iostream>
#include <deque> 
#include <map>

class HD {
public:
  HD(const HD & rhs) = default; // Copy-constructor.
  HD& operator=(const HD & rhs) = default; // Copy-assignment.
  ~HD() = default;  // Destructor
  HD(); // {}
  void AssignProcessToIOqueue(int id);
  void PrintIOqueue(const std::map<int, PCB> & map_of_process) const;
  bool isIOqueueEmpty() const;
  int RemovesFrontFromIOqueue();
  void removeProcessFromIO(int pid);

private:
  std::deque<int> i_o_queue_; // containts the processes ID (int)
};

#endif /* HD_H_ */