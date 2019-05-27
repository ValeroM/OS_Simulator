/*
Author: Marco Valero
Course: Operating Systems
*/


#ifndef PCB_H_
#define PCB_H_

#include <iostream>
#include <string>
#include <vector>

class PCB {
public:
  enum ProcessState {READY, RUNNING, WAITING, TERMINATED, WAITING_FOR_CHILDREN}; 
  // Big 3 as default
  PCB(const PCB & rhs) = default; // Copy-constructor.
  PCB& operator=(const PCB & rhs) = default; // Copy-assignment.
  ~PCB() = default;  // Destructor
  PCB();
  int getHdNumInUse() const;
  void setHdNumInUse(int hd_num);
  void PrintFileName() const; 
  void AssignProcessAsWaitingForChildren();
  void AssignProcessStateAsRunning();
  void AssignProcessStateAsReady(); 
  void AssignProcessStateAsWaiting(); 
  void AssignProcessStateAsTerminated(); 
  bool CheckIfProcessIsWaiting4Children() const; 
  bool CheckIfProcessHasTerminated() const;
  bool CheckIfProcessIsUsingIO() const; 
  bool CheckIfProcessIsRunning() const; 
  int getParentID() const; 
  void SetParentID(int x); 
  void PushChildrenToVectorOfChildren(int children_id); 
  void AssignFileNameToProcess(const std::string & filename); 
  int getChildrenID(int index) const; 
  int getChildrenVectorSize() const; 
  void RemoveAChildrenFromVector(int index); 
  bool isChildrenVectorEmpty() const; 
  void GetCurrentProcessState() const; 
  friend std::ostream& operator<<(std::ostream &out, const PCB & some_process); // DEBUG FUNCTION 
  int getIndexOfProcessInChildVector(int pid) const; 

private:
  std::string file_name_; 
  ProcessState process_state_; // enum 
  int parent_id_; 
  int hd_num_in_use_;
  std::vector<int> children_list_; // since process are being numbered vector is type int 
};

#endif /* PCB_H_ */