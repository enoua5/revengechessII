#include <iostream>
#include <fstream>

#include "nlohmann/json.hpp"
using nlohmann::json;

#include "server/info.h"
#include "server/server.h"


int main()
{
  json settings;
  try
  {
    std::ifstream ifs("conf.json");
    settings = json::parse(ifs,
                           nullptr, // callback
                           true, // allow exceptions
                           true // allow comments - this is the important one
               );
    ifs.close();
  }
  catch(std::exception& e)
  {
    std::cerr << "Something went wrong parsing 'conf.json'." <<
    std::endl << "Note that the file must be in the directory that the server is" <<
    std::endl << "being run *from*, not the directory the executable file is in." <<
    std::endl << std::endl;
    std::cerr << e.what();
    return 1;
  }
  
  try
  {
    bool eula_agreed = settings.at("agreement_accept");
    if(!eula_agreed)
    {
      print_eula();
      return 1;
    }
  }
  catch(std::exception& e)
  {
    std::cerr << "Something went wrong getting the value 'agreement_accept' from 'conf.json'." <<
    std::endl << "Does the value exist in the file?" <<
    std::endl << std::endl;
    std::cerr << e.what();
    return 1;
  }
  
  Server serve(settings);
  
  thread t(bind(&Server::process_messages, &serve));
  
  uint16_t port = 9000;
  try
  {
    port = settings.at("port");
  }
  catch(std::exception& e)
  {
    std::cerr << "*Failed to read port number from settings*" << std::endl;
    std::cerr << e.what() <<
    std::endl << std::endl; 
    // no return
  }
  
  std::cout << "Starting server on port " << port << "..." << std::endl;
  serve.run(port);
  
  t.join();
  
  return 0;
}
