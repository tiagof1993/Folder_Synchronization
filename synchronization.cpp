#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <string>  
#include <sstream>  
#include <ctime>
#include <thread>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <time.h>
#include <dirent.h>
#include <chrono>
#include <ctime>
#include <thread>
using namespace std::chrono_literals;
 

namespace fs = std::filesystem;
//std::string s="";
std::stringstream s;
std::ofstream logFile("log.txt");

template <typename TP>
std::time_t to_time_t(TP tp)
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
              + system_clock::now());
    return system_clock::to_time_t(sctp);
}

std::string convert_fs_time_to_string(fs::file_time_type file_time){
    //fs::file_time_type file_time = fs::last_write_time(__FILE__);
    std::time_t tt = to_time_t(file_time);
    std::tm *gmt = std::gmtime(&tt);
    std::stringstream buffer;
    buffer << std::put_time(gmt, "%A, %d %B %Y %H:%M");
    std::string formattedFileTime = buffer.str();
    std::cout << formattedFileTime << '\n';
    return formattedFileTime;
}

std::string convert_local_time_to_string(){
    std::time_t time = std::time(nullptr);
    std::tm* localtime = std::localtime(&time);
    std::stringstream buffer;
    buffer << std::put_time(localtime, "%A, %d %B %Y %H:%M");
    std::string formattedFileTime = buffer.str();
    std::cout << formattedFileTime << '\n';
    return formattedFileTime;
}

void compare_directories(const fs::path& source,const fs::path& replica, int depth1=0, int depth2=0)
{
    std::string dir1_str="";
    std::string dir2_str="";

    for (auto& p: fs::directory_iterator(source))
    {
        const auto& sourceObject = p.path();
        const auto destinationObject = replica / sourceObject.filename(); 
        //const auto& destinationFolder = q.path();
        //std::cout << "Source Object: " << sourceObject.string() << "\n";
       // std::cout << "Destination Object: " << destinationObject.string() << "\n";
      //  std::cout << std::filesystem::status(sourceObject).permissions() << "\n";

       if(!fs::exists(destinationObject) && fs::is_directory(sourceObject)){
            logFile << sourceObject.string() << " was removed on " << convert_local_time_to_string() << "\n";
            fs::remove_all(destinationObject);
        }
        else if(!fs::exists(destinationObject) && !fs::is_directory(sourceObject)){
           logFile << sourceObject.string() << " was removed on " << convert_local_time_to_string() << "\n";
            fs::remove(destinationObject);
        }

      //If file doesn't exist in the source, it means it was renamed or changed
      if(!fs::exists(sourceObject)){
        logFile << sourceObject.string() << " was renamed on " << convert_local_time_to_string() << "\n";
        break;
      }
      
   if(fs::exists(destinationObject)){
     if(!fs::is_directory(destinationObject)){
    //If the same file has been changed in the source folder more recently than the one on the destination folder
      if(fs::last_write_time(sourceObject) > fs::last_write_time(destinationObject)){
         //convert_fs_time_to_string(fs::last_write_time(sourceObject));
         //convert_fs_time_to_string(fs::last_write_time(destinationObject));
         //const auto copyOptions=std::filesystem::copy_options::overwrite_existing;
        
         logFile << destinationObject.string() << " was updated on " << convert_local_time_to_string() << "\n";
         fs::remove(destinationObject);
         fs::copy(sourceObject,destinationObject,fs::copy_options::overwrite_existing);
        }
     }
     else{
        if(!fs::exists(sourceObject)){
            logFile << destinationObject.string() << " was removed on" << convert_local_time_to_string() << "\n";
            fs::remove_all(destinationObject);
        }
     } 
   }
        else{
            if(fs::is_directory(p.status()) && !fs::is_directory(destinationObject)){
                logFile << destinationObject.string() << " was created on " << convert_local_time_to_string() << "\n";
                fs::create_directory(destinationObject);
            } 
            else{
                logFile << destinationObject.string() << " was updated on " << convert_local_time_to_string() << "\n";
                 fs::copy_file(sourceObject,destinationObject);
            }
        }
    
  
  if(fs::is_directory(p.status())){
             /* if(!fs::is_directory(destinationObject)){
                fs::create_directory(destinationObject);
            }  */
            compare_directories(p.path(),destinationObject,depth1+1,depth2+1);
        }   
  } 

  for(auto& p: fs::directory_iterator(replica)){
        const auto& replicaObject = p.path();
        const auto originalObject = source / replicaObject.filename(); 
        //const auto& destinationFolder = q.path();
        std::cout << "Replica Object: " << replicaObject.string() << "\n";
        std::cout << "Original Object: " << originalObject.string() << "\n";

        //If files are created and then renamed on the original folder
         if(!fs::exists(replicaObject)){
           logFile << replicaObject.string() << " was renamed on " << convert_local_time_to_string() << "\n";
           break;
         }

        //Remove files that do not exist in the source folder
        if(!fs::exists(originalObject) && fs::is_directory(replicaObject)){
            logFile << replicaObject.string() << " was removed on " << convert_local_time_to_string() << "\n";
            fs::remove_all(replicaObject);

        }
        else if(!fs::exists(originalObject) && !fs::is_directory(replicaObject)){
            logFile << replicaObject.string() << " was removed on " << convert_local_time_to_string() << "\n";
            fs::remove(replicaObject);
        }
  }

  }
 
int main(int argc, char** argv)
{
    std::cout << "AA" << std::endl;
    if (argc != 5)
    {
        std::cerr << "Usage: " << argv[0] << " " << argv[1] << " " << argv[2] << "DIRECTORY\n";
        std::exit(EXIT_FAILURE);
    }
    std::string command;
    /*if(!logFile){
        std::cerr << "Problem opening file" << std::endl;
    }*/
   if(!fs::exists(argv[1])){
    fs::create_directory(argv[1]);
   }
   if(!fs::exists(argv[2])){
    fs::create_directory(argv[2]);
   }
   int m=0;
   int sync_time=std::stoi(argv[3]);
   int execution_runs=std::stoi(argv[4]);
   while (m<execution_runs)
   { 
    m++;
    compare_directories(argv[1],argv[2]);
    std::this_thread::sleep_for(std::chrono::milliseconds(sync_time));
   
   }
   logFile.close();
   
    
    
   // if(command=="quit"){
   //    break;
   // }
    //get_timeinfo(argv[1]);
 //}
}