/*
Author: Marco Valero
*/

#include <iostream>
#include <map>

#include "ManageMem.h"

ManageMem::ManageMem(): time_stamp_{0} {}

void ManageMem::setNumOfFrames(int frames_rows) {
  for (int i = 0; i < frames_rows; i++) {
    Frame frame;
    map_of_frames_.insert(std::pair<int, Frame>(i, frame));
  }
}

void ManageMem::increaseTimeStamp() { time_stamp_++; }

int ManageMem::getLeastRecentlyUsed_stampFrameNum() const {
  int result = time_stamp_, frame_num = -1; 
  for (std::map<int, Frame>::const_iterator it = map_of_frames_.begin(); it != map_of_frames_.end(); it++) {
    if (it->second.ts != -1 && it->second.ts < result) { 
      result = it->second.ts;
      frame_num = it->first;
    }
  }
  return frame_num; 
}

void ManageMem::set_a_Frame(int cpu_pid, int log_pg_number) { // uses the next available frame in map 
  if (cpu_pid == 1) {
    std::cout << "Process ID default #1 cannot use memory." << std::endl;
  }
  else { 
    bool success = false; 
    for (auto & map_elem : map_of_frames_) { 
      // if frame row matches pg num and pid matches pid on frame, we just update timestamp
      if (map_elem.second.pg_num == log_pg_number && map_elem.second.pid == cpu_pid) { 
        map_elem.second.ts = time_stamp_;
        increaseTimeStamp();
        success = true; 
        break;
      }
      // else if frame is completely empty 
      else if (map_elem.second.pg_num == -1 && map_elem.second.pid == -1 && map_elem.second.ts == -1) { 
        map_elem.second.pid = cpu_pid;
        map_elem.second.pg_num = log_pg_number;
        map_elem.second.ts = time_stamp_;
        increaseTimeStamp();
        success = true; 
        break;
      }
    } // if it exited out of this loop. then memory frames are all full. We replace the frame by the least recenlty used timestamp 
    if (!success) {  //  if everything is full. then find lru frame row number and replace its contents.
      std::map<int, Frame>::iterator it = map_of_frames_.find(getLeastRecentlyUsed_stampFrameNum());
      it->second.pg_num = log_pg_number;
      it->second.pid = cpu_pid;
      it->second.ts = time_stamp_;
      increaseTimeStamp();
    }
  }
} 

void ManageMem::framesSnapShot() const { // snapshot of memory
  for (auto const& map_elem : map_of_frames_) { 
    if (map_elem.second.pg_num == -1 && map_elem.second.pid == -1 && map_elem.second.ts == -1) { 
      continue;
    }
    else { 
      std::cout << "Frame #: " << map_elem.first << " | " << "Pg #: " << map_elem.second.pg_num << " | " << "Pid: " << map_elem.second.pid << " | " << "Ts: " << map_elem.second.ts << std::endl;
    }
  }
}

void ManageMem::clearFrames(int p_id) {
  for (auto & map_elem : map_of_frames_) { 
    if (map_elem.second.pg_num == -1 && map_elem.second.pid == -1 && map_elem.second.ts == -1) { 
      continue;
    }
    else if (map_elem.second.pid == p_id) {
      map_elem.second.pg_num = -1; 
      map_elem.second.pid = -1; 
      map_elem.second.ts = -1;
    }
  }
}