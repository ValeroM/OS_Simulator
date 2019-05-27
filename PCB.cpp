#include <iostream>
#include <string>
#include <vector>

#include "PCB.h"

// since 1 is the main process running, parent id is 1 by default  
// if the process has -1 as the number disk, then it is not using any hard disks.
PCB::PCB() : file_name_{""}, process_state_{READY}, parent_id_{1}, hd_num_in_use_{-1} { }

int PCB::getHdNumInUse() const {
  return hd_num_in_use_;
}

void PCB::setHdNumInUse(int hd_num) {
  hd_num_in_use_ = hd_num;
}

void PCB::PrintFileName() const {
  std::cout << file_name_;
}

void PCB::AssignProcessAsWaitingForChildren() {
  process_state_ = WAITING_FOR_CHILDREN;
}

void PCB::AssignProcessStateAsRunning() {
  process_state_ = RUNNING;
}

void PCB::AssignProcessStateAsReady() {
  process_state_ = READY;
}

void PCB::AssignProcessStateAsWaiting() {
  process_state_ = WAITING;
}

void PCB::AssignProcessStateAsTerminated() {
  process_state_ = TERMINATED;
}

bool PCB::CheckIfProcessIsWaiting4Children() const {
  if (process_state_ == WAITING_FOR_CHILDREN) 
    return true; 
  return false; 
}

bool PCB::CheckIfProcessHasTerminated() const {
  if (process_state_ == TERMINATED) 
    return true; 
  return false; 
}

bool PCB::CheckIfProcessIsUsingIO() const {
  if (process_state_ == WAITING) 
    return true; 
  return false; 
}

bool PCB::CheckIfProcessIsRunning() const {
  if (process_state_ == RUNNING) 
    return true; 
  return false; 
}

int PCB::getParentID() const {
  return parent_id_;
}

void PCB::SetParentID(int x) {
  parent_id_ = x;
}

void PCB::PushChildrenToVectorOfChildren(int children_id) {
  children_list_.push_back(children_id);
}

void PCB::AssignFileNameToProcess(const std::string & filename) {
  file_name_ = filename; 
}

int PCB::getChildrenID(int index) const {
  return children_list_[index];
}

int PCB::getChildrenVectorSize() const {
  return children_list_.size();
}

void PCB::RemoveAChildrenFromVector(int index) {
  children_list_.erase(children_list_.begin() + index);
}

bool PCB::isChildrenVectorEmpty() const {
  if (children_list_.empty())
    return true;
  return false; 
}

void PCB::GetCurrentProcessState() const {
  if (process_state_ == READY) { 
    std::cout << "Ready" << std::endl;
  }
  else if (process_state_ == RUNNING) { 
    std::cout << "Running" << std::endl;
  }
  else if (process_state_ == WAITING) { 
    std::cout << "Waiting" << std::endl;
  }
  else if (process_state_ == TERMINATED) { 
    std::cout << "Terminated" << std::endl;
  }
  else if (process_state_ == WAITING_FOR_CHILDREN) { 
    std::cout << "Waiting for Children" << std::endl;
  }
}

std::ostream& operator<<(std::ostream &out, const PCB & some_process) { // DEBUG FUNCTION 
  out << "My parent id is: " << some_process.parent_id_ << " my filename is: " << some_process.file_name_ << " and my process state is: "; 
  some_process.GetCurrentProcessState();
  return out;
}

int PCB::getIndexOfProcessInChildVector(int pid) const {
  int index = 0;
  for (size_t i = 0; i < children_list_.size(); i++) { 
    if (children_list_[i] == pid) { 
      index = static_cast<int>(i); 
      break;
    }
  }
  return index; 
}