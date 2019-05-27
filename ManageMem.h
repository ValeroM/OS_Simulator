/*
Author: Marco Valero
*/

#ifndef ManageMem_H_
#define ManageMem_H_

#include <iostream>
#include <map>

class ManageMem {
public:
  ManageMem(const ManageMem & rhs) = default; // Copy-constructor.
  ManageMem& operator=(const ManageMem & rhs) = default; // Copy-assignment.
  ~ManageMem() = default;  // Destructor
  ManageMem();
  void setNumOfFrames(int frames_rows);
  void increaseTimeStamp();
  int getLeastRecentlyUsed_stampFrameNum() const;
  void set_a_Frame(int cpu_pid, int log_pg_number);
  void framesSnapShot() const;
  void clearFrames(int p_id);

private:
  int time_stamp_;
  struct Frame { 
    int pg_num, pid, ts;
    Frame() { pg_num = -1; pid = -1; ts = -1; } // set to -1 by default 
  };
  std::map<int, Frame> map_of_frames_; //  map of frames where the int is the frame #
};

#endif /* HD_H_ */