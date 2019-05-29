/*
Author: Marco Valero
Course: Operating Systems
May 11th, 2019 
*/

#ifndef OS_H_
#define OS_H_

#include <iostream>
#include <string>
#include <deque> 
#include <map>

#include "PCB.h"
#include "HD.h"
#include "ManageMem.h"

class OS {
public:
  // Big 3 as default
  OS(const OS & rhs) = default; // Copy-constructor.
  OS& operator=(const OS & rhs) = default; // Copy-assignment.
  ~OS() = default;  // Destructor
  OS();
  void AssignIdToProcessAndPlaceToRQ(const PCB & process);
  void S_rSnapshot() const;
  void RoundRobin();
  void ForkingFromProcessInCPU(const PCB & process);
  void SetNumOfHardDisks(int hd_num);
  void RequestIOqueue(int hd_num, const std::string & filename);
  void S_iSnapshot() const;
  int getCPU() const;
  int getCurrentProcessID() const; 
  void exitCommand();  
  void waitCommand();
  void AssignNextProcessInRQtoCPU();
  void finishedIOqueue(int hd_num);
  bool removeAprocessFromRQ(int pid);
  void removeProcessOnIOBasedOnPid(int pid_target);
  unsigned long int getRamMem() const;
  unsigned int getPgSize() const;
  void setMem(unsigned long int ram_mem, unsigned int pg_size);
  void requestMemAddress(int log_address);
  void memSnapShot() const;
  void Destroy(int target_pid); // recursively destroy all descendants 
  
private:
  std::map<int, PCB> map_of_process_; 
  std::deque<int> ready_queue_; 
  int current_process_id_;
  int hard_disk_nums_; 
  std::map<int, HD> map_of_hard_disks_; 
  int CPU; // simulating CPU usage. Holds pid of process using the cpu
  unsigned long int main_ram_mem_;
  unsigned int main_pg_size_;
  ManageMem manage_mem_;
};

#endif /* OS_H_ */
