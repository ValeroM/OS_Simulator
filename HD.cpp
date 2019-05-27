/*
Author: Marco Valero
*/


#include <iostream>
#include <deque> 
#include <map>

#include "PCB.h"
#include "HD.h"

HD::HD() {}

void HD::AssignProcessToIOqueue(int id) {
  i_o_queue_.push_back(id);
}

void HD::PrintIOqueue(const std::map<int, PCB> & map_of_process) const {
  for (const auto &p_id : i_o_queue_) { 
    std::map<int, PCB>::const_iterator it = map_of_process.find(p_id);
    std::cout << "process id " << p_id << " --> reading/writing: "; 
    it->second.PrintFileName();
    std::cout << std::endl; 
  }
}

bool HD::isIOqueueEmpty() const {
  if (i_o_queue_.empty()) 
    return true; 
  return false; 
}

int HD::RemovesFrontFromIOqueue() {
  int pid = 0; 
  if (!i_o_queue_.empty()) { 
    pid = i_o_queue_.front();
    i_o_queue_.pop_front();
  }
  return pid; 
}

void HD::removeProcessFromIO(int pid) {
  for (std::deque<int>::iterator it = i_o_queue_.begin(); it != i_o_queue_.end(); it++) { 
    if (*it == pid) {
      i_o_queue_.erase(it);
      break;
    }
  }
}