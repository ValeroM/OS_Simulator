/*
Author: Marco Valero
Course: Operating Systems
May 11th, 2019 
*/

#include <iostream>
#include <string>
#include <deque> 
#include <map>

#include "PCB.h"
#include "HD.h"
#include "OS.h"
#include "ManageMem.h"

// cpu by dafault is one. Processes ID's start at 2
// The enumeration of the hard disks starts with 0.
OS::OS () : current_process_id_{1}, hard_disk_nums_{-1}, CPU{1}, main_ram_mem_{0}, main_pg_size_{0} {
  std::cout << "OPERATING SYSTEM IS NOW ACTIVE" << std::endl;
}

void OS::AssignIdToProcessAndPlaceToRQ(const PCB & process) { 
  current_process_id_++; // id for newly created process 
  map_of_process_.insert(std::pair<int, PCB>(current_process_id_, process));
  std::map<int, PCB>::iterator it = map_of_process_.find(current_process_id_);
  if (CPU == 1) {  // if cpu is idle, put newly created process in cpu 
    CPU = it->first; 
    it->second.AssignProcessStateAsRunning();
  } 
  else { 
    ready_queue_.push_back(it->first); // if not idle, new pid goes to the end of the ready queue 
  }
}

void OS::S_rSnapshot() const {
  if (CPU >= 2) { 
    std::cout << "Current process using the CPU: pid " << CPU << std::endl;
    if (!ready_queue_.empty()) { 
      std::cout << "Processes in the ready-queue:" << std::endl;
      for (std::deque<int>::const_iterator it = ready_queue_.begin(); it != ready_queue_.end(); it++) { 
        std::cout << "pid " << *it << std::endl;
      }
    } 
    else {
      std::cout << "Ready-queue is empty" << std::endl;
    }
  }
  else if (CPU == 1) {
    std::cout << "CPU is idle. As default, it has the idle number: #1" << std::endl;
  }
}

void OS::RoundRobin() {
  // if r-q is not empty, assign the pid next in line the cpu. if there is no one in line, make no changes. pid using cpu stays using it
  if (!ready_queue_.empty()) { 
    std::map<int, PCB>::iterator it = map_of_process_.find(CPU);
    ready_queue_.push_back(CPU); // push pid using cpu to the back of r-q 
    it->second.AssignProcessStateAsReady(); // assign it as ready (since its waiting in r-q)
    CPU = ready_queue_.front(); // CPU now gets the next process in line 
    it = map_of_process_.find(CPU); 
    it->second.AssignProcessStateAsRunning(); // Set the current process using the CPU as running 
    ready_queue_.pop_front(); // pop the front of our deque, since the pid is already in CPU.
  } 
}

void OS::ForkingFromProcessInCPU(const PCB & process) {
  if (CPU <= 1) {
    std::cout << "Process default #1 cannot fork a child" << std::endl;
  }
  else { 
    current_process_id_++;  // creates new process 
    map_of_process_.insert(std::pair<int, PCB>(current_process_id_, process)); 
    std::map<int, PCB>::iterator it2 = map_of_process_.find(current_process_id_); // newly created process 
    std::map<int, PCB>::iterator it = map_of_process_.find(CPU);
    it->second.PushChildrenToVectorOfChildren(current_process_id_); // pushes children to vector of children of the parent 
    it2->second.SetParentID(CPU); // assign the parent id to the newly creates process 
    ready_queue_.push_back(it2->first);  // push the new pid to the back of the deque 
  }
}

void OS::SetNumOfHardDisks(int hd_num) {
  for (int i = 0; i < hd_num; i++) {
    HD hard_disk; 
    hard_disk_nums_++; // this first increment goes from -1 to 0;
    map_of_hard_disks_.insert(std::pair<int, HD>(hard_disk_nums_, hard_disk));  
  }
}

void OS::RequestIOqueue(int hd_num, const std::string & filename) {
  std::map<int, HD>::iterator it = map_of_hard_disks_.find(hd_num); // find disk num requested in my map of disks 
  if (getCPU() != 1) { 
    std::map<int, PCB>::iterator it2 = map_of_process_.find(CPU); // getting the current process in CPU 
    it->second.AssignProcessToIOqueue(CPU);  // process that was in CPU goes to i/o queue of specific hd_num; 
    it2->second.AssignProcessStateAsWaiting(); // and make the process go to waiting state 
    it2->second.AssignFileNameToProcess(filename);  // and assign the process its file name 
    it2->second.setHdNumInUse(it->first); // keep track of what hd num the pid is using. important for exit command. 
    AssignNextProcessInRQtoCPU(); // keep the r-q moving 
  }
}

void OS::S_iSnapshot() const {
  for (int i = 0; i <= hard_disk_nums_; i++) {
    std::map<int, HD>::const_iterator it = map_of_hard_disks_.find(i); // find disk num in map 
    if (it->second.isIOqueueEmpty()) { 
      std::cout << "Disk #" << i << " is idle" << std::endl;
    }
    else { 
      std::cout << "Disk #" << i << " contains: "; 
      it->second.PrintIOqueue(map_of_process_); 
    }
  }
}

int OS::getCPU() const { return CPU; }
  
int OS::getCurrentProcessID() const {  return current_process_id_; }

void OS::exitCommand() {
  if (CPU != 1) { // pid default #1 should never call exit but as precaution. 
    std::map<int, PCB>::iterator it = map_of_process_.find(CPU); // getting the current process in CPU 
    int parent_id = it->second.getParentID();
    if (parent_id == 1) { // if its parent is the default p_id, assume deafult process id 1 is always waiting. no need to make 1 zombie
      if (!it->second.isChildrenVectorEmpty()) {  // if it has kids, delete all kids 
        for (int i = 0; i < it->second.getChildrenVectorSize(); i++) {
          int c_id = it->second.getChildrenID(i); // gets the child id 
          std::map<int, PCB>::iterator c_it = map_of_process_.find(c_id); 
          if (c_it->second.CheckIfProcessIsWaiting4Children()) {
            // some children of children may be waiting, but since its parent called exit, we push it to r-q and Destroy() will remove it.
            ready_queue_.push_back(c_id); 
          }
          Destroy(c_id);
        }
      }
      AssignNextProcessInRQtoCPU(); // advance the r-q 
      manage_mem_.clearFrames(it->first); // clear memory for the pid that called exit. 
      map_of_process_.erase(it); // erases the process that calls exit  
    }
    else if (parent_id >= 2) { // if this process does in fact have a parent and parent isn't the defult pid (#1)
      std::map<int, PCB>::iterator parent_it = map_of_process_.find(parent_id); // getting parent process from map. parent has pid >= 2
      if (parent_it->second.CheckIfProcessIsWaiting4Children()) {  // CHECK if parent of currently running process is already waiting 
        // if so, terminitate the running process and all of its kids 
        // if the child that is terminating has kids: terminate all of its kids as well 
        if (!it->second.isChildrenVectorEmpty()) { // 'it' is the guy that called exit, parent-it is the parent of the guy that called exit 
          for (int i = 0; i < it->second.getChildrenVectorSize(); i++) {
            int c_id = it->second.getChildrenID(i); // get the id of the childrens of the children; takes as parameter an index 
            std::map<int, PCB>::iterator children_of_children_it = map_of_process_.find(c_id);  
            if (children_of_children_it->second.CheckIfProcessIsWaiting4Children()) {
              ready_queue_.push_back(c_id); 
            }
            Destroy(c_id);
          }
        }
        parent_it->second.RemoveAChildrenFromVector(parent_it->second.getIndexOfProcessInChildVector(it->first)); // process that called exit must be removed from the vector of children of its parent 
        manage_mem_.clearFrames(it->first); // destroys the memory frames of the guy that called exit
        map_of_process_.erase(it); // erases the process that called exit
        parent_it->second.AssignProcessStateAsReady(); // set the parent as ready and ..
        ready_queue_.push_back(parent_it->first); // since the parent is runnable again, we put it to the end of the r q
        AssignNextProcessInRQtoCPU(); // move the rq, so cpu gets next one.
      }
      else { // if parent hasnt called wait yet, the process turns into zombie. but before turning to zombie, we delete all of its descendants
        for (int i = 0; i < it->second.getChildrenVectorSize(); i++) {
          int c_id = it->second.getChildrenID(i); // get the id of the childrens of the children; takes as parameter an index 
          std::map<int, PCB>::iterator children_of_children_it = map_of_process_.find(c_id);  
          if (children_of_children_it->second.CheckIfProcessIsWaiting4Children()) {
            ready_queue_.push_back(c_id);  
          }
          Destroy(c_id);
        }
        manage_mem_.clearFrames(it->first); // clear mem of zombie pid since its terminated.
        it->second.AssignProcessStateAsTerminated(); // turns zombie in case parent calls wait later 
        AssignNextProcessInRQtoCPU();
      } 
    }
  }
}

void OS::waitCommand() {
  bool a_child_is_terminated = false; 
  std::map<int, PCB>::iterator it = map_of_process_.find(CPU); 
  std::map<int, PCB>::iterator children_it;
  if (it != map_of_process_.end() && !it->second.isChildrenVectorEmpty()) {  // if children vector is not empty 
    int index = 0; 
    for (int i = 0; i < it->second.getChildrenVectorSize(); i++) {
      int c_id = it->second.getChildrenID(i); // getChildrenId() is based on index cuz of children vector 
      children_it = map_of_process_.find(c_id);  // gets the children process 
      if (children_it->second.CheckIfProcessHasTerminated()) { // if 1 kid has termindated (zombie), get rid of zombie and dad keeps running
        a_child_is_terminated = true; 
        index = i;
        break;
      }
    }
    if (a_child_is_terminated) {
      it->second.RemoveAChildrenFromVector(index); // delete children from the children vector of its parent. 
      map_of_process_.erase(children_it); // also erases the process from the map of processes. 
    }
    if (!a_child_is_terminated) { // if there is someone to wait for, meaning not any children are terminated yet
      it->second.AssignProcessAsWaitingForChildren(); // assign the dad as waiting for kid to terminate. the kid will eventually call exit 
      AssignNextProcessInRQtoCPU();  // and make it leave the cpu 
    }
  } 
  else {
    std::cout << "Currently running process does not have any children." << std::endl;
  }
}

void OS::AssignNextProcessInRQtoCPU() {
  if (!ready_queue_.empty()) { 
    CPU = ready_queue_.front();
    std::map<int, PCB>::iterator it_ = map_of_process_.find(CPU); 
    it_->second.AssignProcessStateAsRunning(); 
    ready_queue_.pop_front(); 
  } 
  else { 
    CPU = 1; // 1 is the default process. In other words, cpu is idle. 
  }
}

void OS::finishedIOqueue(int hd_num) {
  std::map<int, HD>::iterator hd_it = map_of_hard_disks_.find(hd_num); // find disk num in our map. 
  int process_id = hd_it->second.RemovesFrontFromIOqueue(); // removes the pid from the hd#  i/o queue and returns the id of that process
  std::map<int, PCB>::iterator process_it = map_of_process_.find(process_id); 
  process_it->second.AssignProcessStateAsReady(); 
  process_it->second.setHdNumInUse(-1); // reset the hd num to the default one. 
  if (CPU == 1) { // if cpu happens to be idle, you assign it to start using the cpu 
    CPU = process_it->first;
    process_it->second.AssignProcessStateAsRunning();
  }
  else { 
    ready_queue_.push_back(process_it->first); // if not idle, it gets push back to r q 
  }
}

bool OS::removeAprocessFromRQ(int pid) {
  std::deque<int>::iterator it;
  for (it = ready_queue_.begin(); it != ready_queue_.end(); it++) {
    if (pid == *it) { 
      ready_queue_.erase(it);
      return true;
    }
  }
  return false; 
}

void OS::removeProcessOnIOBasedOnPid(int pid_target) {
  std::map<int, PCB>::iterator process_it = map_of_process_.find(pid_target); 
  if (process_it->second.getHdNumInUse() != -1) { // must be a valid hd number. 
    std::map<int, HD>::iterator it = map_of_hard_disks_.find(process_it->second.getHdNumInUse());  
    it->second.removeProcessFromIO(pid_target);
  }
}

unsigned long int OS::getRamMem() const { return main_ram_mem_; }

unsigned int OS::getPgSize() const { return main_pg_size_; }

void OS::setMem(unsigned long int ram_mem, unsigned int pg_size) {
  main_ram_mem_ = ram_mem;
  main_pg_size_ = pg_size;
  long frames_temp = getRamMem()/getPgSize(); // deciding all the memory frames ram will have 
  int frames = static_cast<int>(frames_temp);
  manage_mem_.setNumOfFrames(frames);
}

void OS::requestMemAddress(int log_address) {
  unsigned int pg_num_temp = static_cast<unsigned int>(log_address)/getPgSize();
  int log_pg_num = static_cast<int>(pg_num_temp); // chooses the memory page (floor)
  manage_mem_.set_a_Frame(getCPU(), log_pg_num);
}

void OS::memSnapShot() const {
  manage_mem_.framesSnapShot();
}

void OS::Destroy(int target_pid) {
  std::map<int, PCB>::iterator process_it = map_of_process_.find(target_pid); 
  bool already_a_zombie = false;  
  if (process_it->second.CheckIfProcessHasTerminated()) {
    map_of_process_.erase(process_it); // erases the process if the targetpid is already a zombie. (zombie = already erased from everywhere)
    already_a_zombie = true;  
  }
  if (!already_a_zombie) { 
    manage_mem_.clearFrames(target_pid); // clears frames for such pid 
    bool done = (CPU == target_pid); // checking if pid is in the cpu, if so, it won't be in i/o or r-q
    if (!done){
      done = removeAprocessFromRQ(target_pid); // if its not in r-q queue, it's in i/o queue
    }
    if (!done) { 
      removeProcessOnIOBasedOnPid(target_pid);
    }
    for (int i = 0; i < process_it->second.getChildrenVectorSize(); i++) { 
      Destroy(process_it->second.getChildrenID(i)); // destroy all descendants recursively 
    }
    map_of_process_.erase(process_it);  
  }
}
