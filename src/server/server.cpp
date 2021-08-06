#include <string_view>
#include <fstream>
#include <filesystem>

#include "server/server.h"
const auto& ws_text = websocketpp::frame::opcode::text;

#include "nlohmann/json.hpp"
using nlohmann::json;

Version Server::version = Version("Revenge Chess Server, standard compliant", 'x', 0, 0, 1);
Version Server::minimum_client_version = Version("standard compliant", 'x', 2, 2, 1);

connection_info::connection_info()
{
  user_name = "guest";
}

bool Server::verify_compatible_version(json version, connection_hdl conn)
{
  try
  {
    std::string_view v_name = std::string_view((std::string)version.at("name"));
    if(!v_name.ends_with(Server::minimum_client_version.name))
      return false;
      
    unsigned int v_major = version.at("major");
    if(v_major < Server::minimum_client_version.major)
      return false;
    if(v_major > Server::minimum_client_version.major)
      return true;
      
    unsigned int v_minor = version.at("minor");
    if(v_minor < Server::minimum_client_version.minor)
      return false;
    if(v_minor > Server::minimum_client_version.minor)
      return true;
      
    unsigned int v_patch = version.at("patch");
    if(v_patch < Server::minimum_client_version.patch)
      return false;
    return true;
  }
  catch(std::exception& e)
  {
    sendError(conn, "Error parsing version number");
  }
  return false;
}

void Server::sendError(connection_hdl conn, std::string message, bool close)
{
  json res = {
    {"res", "error"},
    {"error", message}
  };
  
  endpoint.send(conn, res.dump(), ws_text);

  if(close)
    endpoint.close(conn, websocketpp::close::status::invalid_payload, message);
}

Server::Server(json settings)
{
  this->settings = settings;

  endpoint.init_asio();
  
  endpoint.set_open_handler(bind(&Server::on_open, this, ::_1));
  endpoint.set_close_handler(bind(&Server::on_close, this, ::_1));
  endpoint.set_message_handler(bind(&Server::on_message, this, ::_1, ::_2));
}

void Server::run(uint16_t port)
{
  endpoint.listen(port);
  endpoint.start_accept();
  endpoint.run();
}

void Server::on_open(connection_hdl hdl)
{
  lock_guard<mutex> guard(action_lock);
  actions.push(action(SUBSCRIBE, hdl));
  action_cond.notify_one();
}

void Server::on_close(connection_hdl hdl)
{
  lock_guard<mutex> guard(action_lock);
  actions.push(action(UNSUBSCRIBE, hdl));
  action_cond.notify_one();
}

void Server::on_message(connection_hdl hdl, server::message_ptr msg)
{
  lock_guard<mutex> guard(action_lock);
  actions.push(action(MESSAGE, hdl, msg));
  action_cond.notify_one();
}

void Server::process_messages()
{
  while(true)
  {
    unique_lock<mutex> lock(action_lock);
    
    while(actions.empty())
      action_cond.wait(lock);
          
    action a = actions.front();
    actions.pop();
      
    lock.unlock();
    
    if(a.type == SUBSCRIBE)
    {
      lock_guard<mutex> guard(connection_lock);
      connections.insert(std::pair<connection_hdl, connection_info>(a.hdl, connection_info()));
      
      try
      {
        json resp = {
          {"res", "conn_success"},
          {"name", connections.at(a.hdl).user_name}
        };
        endpoint.send(a.hdl, resp.dump(), ws_text);
      }
      catch(std::exception& e)
      {
        std::cerr << "Failed to get user id in SUBSCRIBE" << std::endl;
      }
    }
    else if(a.type == UNSUBSCRIBE)
    {
      lock_guard<mutex> guard(connection_lock);
      connections.erase(a.hdl);
    }
    else if(a.type == MESSAGE)
    {
      lock_guard<mutex> guard(connection_lock);
      try
      {
        json message = json::parse(a.msg->get_payload());
        std::string req = message.at("req");
        respond(a.hdl, req, message);
      }
      catch(std::exception& e)
      {
        sendError(a.hdl, "Failed to parse request");
      }
    }
  }
}

void Server::respond(connection_hdl conn, std::string req, json full)
{
  try
  {
    if(req == "version")
    {
      if(!verify_compatible_version(full.at("client_version"), conn))
      {
        sendError(conn, "Incompatible version", true);
        return;
      }
      
      json response = {
        {"res", "version"},
        {"version",
          {
            {"name", version.name},
            {"major", version.major},
            {"minor", version.minor},
            {"patch", version.patch}
          }
        }
      };
      
      endpoint.send(conn, response.dump(), ws_text);
    }
    else if(req == "user_set")
    {
      // TODO? make an option requiring unique user names.
      connections.at(conn).user_name = full.at("name");
      json response = {
        {"res", "user_set"},
        {"name", full.at("name")}
      };
      endpoint.send(conn, response.dump(), ws_text);
    }
    
    else
      sendError(conn, "Unrecognized request");
  }
  catch(std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    sendError(conn, "Error generating response");
  }
}

