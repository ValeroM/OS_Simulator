/*
Author: Marco Valero
Course: Operating Systems
*/

#include <iostream>
#include <string>
#include <regex>

#include "PCB.h"
#include "OS.h"


bool AskUserForInputsAndValidate(std::string & hd_nums_str, std::string & ram_mem_str, std::string &size_of_page_str);
bool ValidateUserInputs(const std::string & input, const std::regex & reg);
void DisplayInvalidInputError();
bool HDnumWithinBounds(int hdnum, int number_of_hard_disks);

int main() {
  std::string user_input, hd_nums_str, ram_mem_str, size_of_page_str;
  unsigned long int ram_mem = 0;
  unsigned int size_of_page = 0;
  int number_of_hard_disks = 0;
  // The enumeration of memory pages && hard disks start start from 0.
  if (AskUserForInputsAndValidate(hd_nums_str, ram_mem_str, size_of_page_str)) { 
    OS main_os;
    ram_mem = std::stoul(ram_mem_str); 
    size_of_page = std::stoi(size_of_page_str); 
    number_of_hard_disks = std::stoi(hd_nums_str); 
    main_os.setMem(ram_mem, size_of_page); // sets all mem frames the o.s will have 
    main_os.SetNumOfHardDisks(number_of_hard_disks);
    std::cin.ignore(256, '\n');
    while (true) {
      std::getline(std::cin, user_input);
      std::regex regA("A");
      std::regex regQ("Q");
      std::regex regfork("fork");
      std::regex regexit("exit");
      std::regex regwait("wait");
      std::regex reg_d_number_file_name("d\\s(\\d+)\\s([^\\s]+)");
      std::regex reg_D_number("D\\s(\\d+)");
      std::regex reg_m_address("m\\s(\\d+)");
      std::smatch matches; // to get groups 
      std::regex regSr("S r");
      std::regex regSi("S i");
      std::regex regSm("S m");
      if (regex_match(user_input, regA)) { 
        PCB process; 
        main_os.AssignIdToProcessAndPlaceToRQ(process);  
      }
      else if (regex_match(user_input, regQ)) { 
        main_os.RoundRobin(); 
      }
      else if (regex_match(user_input, regSr)) { 
        main_os.S_rSnapshot();
      }
      else if (regex_match(user_input, regexit)) { 
        main_os.exitCommand();
      }
      else if (regex_match(user_input, regwait)) { 
        main_os.waitCommand();
      }
      else if (regex_match(user_input, matches, reg_d_number_file_name)) {
        int disk_num = std::stoi(matches[1].str()); // getting the disk number
        std::string filename = matches[2].str(); // getting the file name 
        if (HDnumWithinBounds(disk_num, number_of_hard_disks)) { 
          main_os.RequestIOqueue(disk_num, filename); 
        }
      }
      else if (regex_match(user_input, matches, reg_D_number)) { // hd # has finished the work for 1 process.
        int disk_num = std::stoi(matches[1].str()); // getting the disk number
        if (HDnumWithinBounds(disk_num, number_of_hard_disks)) { 
          main_os.finishedIOqueue(disk_num);
        }
      }
      else if (regex_match(user_input, matches, reg_m_address)) {
        int logical_address = std::stoi(matches[1].str()); // getting the number of mem requested
        main_os.requestMemAddress(logical_address);
      }
      else if (regex_match(user_input, regSi)) { 
        main_os.S_iSnapshot();
      }
      else if (regex_match(user_input, regSm)) { 
        main_os.memSnapShot();
      }
      else if (regex_match(user_input, regfork)) { 
        PCB process; 
        main_os.ForkingFromProcessInCPU(process);
      }
      else {
        DisplayInvalidInputError();
      }
    }
  } 
  else {
    std::cout << "Invalid size inputs." << std::endl;
  }
  return 0;
}

bool AskUserForInputsAndValidate(std::string & hd_nums_str, std::string & ram_mem_str, std::string &size_of_page_str) {
  std::cout << "Enter how much RAM memory is there on the simulated computer." << std::endl;
  std::cin >> ram_mem_str;
  std::regex int_regex_1("\\d+");
  std::cout << "Enter the size of the page/frame." << std::endl;
  std::cin >> size_of_page_str;
  std::regex int_regex_2("\\d+");
  std::cout << "Enter the number of hard disks the simulated computer have." << std::endl;
  std::cin >> hd_nums_str;
  std::regex int_regex_3("\\d+");
  bool valid_inputs = (ValidateUserInputs(ram_mem_str, int_regex_1) && ValidateUserInputs(size_of_page_str, int_regex_2) && ValidateUserInputs(hd_nums_str, int_regex_3));
  return valid_inputs; 
}

bool ValidateUserInputs(const std::string & input, const std::regex & reg) {
  if (regex_match(input, reg)) 
    return true;
  return false; 
}

void DisplayInvalidInputError() {
  std::cout << "Invalid Keywords. Please try again." << std::endl;
}

bool HDnumWithinBounds(int hdnum, int number_of_hard_disks) {
  if (hdnum >= 0 && hdnum < number_of_hard_disks) 
    return true; 
  else { 
    std::cout << "Hard disk number must be less than " << number_of_hard_disks << " and greater than 0." << std::endl;
    return false; 
  }
}