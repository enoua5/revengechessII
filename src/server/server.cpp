#include <string_view>
#include <fstream>
#include <filesystem>

#include "server/server.h"
#include "server/gameid.h"

const auto& ws_text = websocketpp::frame::opcode::text;

#include "nlohmann/json.hpp"
using nlohmann::json;

// When making forks of this project, version names should end in
// "standard compliant" if they are compatible with the vanilla client
Version Server::version = Version("Revenge Chess Server, standard compliant", 'x', 0, 0, 2);
// change the version name here if the server should reject the vanilla client
Version Server::minimum_client_version = Version("standard compliant", 'x', 2, 2, 2);

connection_info::connection_info()
{
  id = ++ connection_info::next_id;
  user_name = "guest";
  current_game = "";
}

uint32_t connection_info::next_id = 0;

uint32_t connection_info::get_id()
{
  return id;
}

bool connection_info::operator==(const connection_info& other)
{
  return other.id == this->id;
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
      // TODO perhaps we could allow game IDs to be included in a
      // resource path, allowing users to join with URL.
      // From stackoverflow:
      // server::connection_ptr con = s.get_con_from_hdl(hdl);
      // std::string path = con->get_resource();
    
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
      try
      {
        endGame(connections.at(a.hdl).current_game);
      }
      catch(std::exception& e){}
      
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

ServerGame::ServerGame(Server* _server, connection_hdl hdl, bool _priv, PlayerColor join_as,
    unsigned int _startingTime,
    unsigned int _increment,
    IncrementMethod _inct)
    : priv(_priv), started(false), 
    black_req_rematch(false), white_req_rematch(false),
    startingTime(_startingTime), increment(_increment), inct(_inct),
    server(_server)
{
  if(join_as == WHITE)
  {
    white = hdl;
    white_in = true;
  }
  else
  {
    black = hdl;
    black_in = true;
  }

  creation_time = time_to_mill(get_current_time());
}

bool ServerGame::join(connection_hdl hdl)
{
  // TODO make sure players can't join their own game
  bool joined = false;
  if(!white_in)
  {
    white = hdl;
    white_in = true;
    joined = true;
  }
  
  else if(!black_in)
  {
    black = hdl;
    black_in = true;
    joined = true;
  }
  
  if(joined)
    start();
  return joined;
}

void ServerGame::start()
{
  game.board = Board();
  game.clock = Clock(startingTime, increment, inct);
  started = true;
  white_req_rematch = false;
  black_req_rematch = false;
}

bool ServerGame::rematch(connection_hdl conn)
{
  if(!started)
  {
    return false;
  }
  
  if(server->conEq(conn, white))
  {
    white_req_rematch = true;
  }
  else if(server->conEq(conn, black))
  {
    black_req_rematch = true;
  }
  else
  {
    return false;
  }
  
  if(white_req_rematch && black_req_rematch)
  {
    connection_hdl swap = white;
    white = black;
    black = swap;
    start();
    return true;
  }
  
  return false; 
}

unsigned int ServerGame::get_creation_time(){return creation_time;}

void Server::endGame(std::string id)
{
  if(id == "")
    return;
  
  json response = {
    {"res", "game_closed"},
    {"id", id}
  };
  
  try
  {
    connection_hdl black = games.at(id).black;
    endpoint.send(black, response.dump(), ws_text);
    connections.at(black).current_game = "";
  }
  catch(std::exception& e){}
  try
  {
    connection_hdl white = games.at(id).white;
    endpoint.send(white, response.dump(), ws_text);
    connections.at(white).current_game = "";
  }
  catch(std::exception& e){}
  
  // no need to check if it actually exists, .erase() doesn't care.
  games.erase(id);
}

bool Server::conEq(connection_hdl a, connection_hdl b)
{
  return connections.at(a) == connections.at(b);
}

void Server::respond(connection_hdl conn, std::string req, json full)
{
  try
  {
    ////////////////////////////////////////////////////////////////////////////
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
    ////////////////////////////////////////////////////////////////////////////
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
    ////////////////////////////////////////////////////////////////////////////
    else if(req == "create_game")
    {
      //connection_hdl, bool priv, PlayerColor join_as,
      //unsigned int startingTime,
      //unsigned int increment,
      //IncrementMethod inct
      
      // parse the json arguments first, so if there's an error, we can deal with it quick
      bool priv = full.at("private");
      bool as_white = full.at("as_white");
      PlayerColor join_as = as_white ? WHITE : BLACK;
      unsigned int startingTime = full.at("starting_time");
      unsigned int increment = full.at("increment");
      std::string inct_s = full.at("inct");
      IncrementMethod inct = stringToInct(inct_s);
        
      // generate a game ID
      uint32_t uid = connections.at(conn).get_id();
      std::string id = gameIdGenerator::generate_id(uid);
      
      // create the game
      games.insert(std::pair<std::string, ServerGame>(id, ServerGame(
        this, conn, priv, join_as, startingTime, increment, inct
      )));
      
      // set the player's current game
      endGame(connections.at(conn).current_game);
      connections.at(conn).current_game = id;
      
      // tell the user
      json response = {
        {"res", "game_created"},
        {"id", id}
      };
      endpoint.send(conn, response.dump(), ws_text);
    }
    ////////////////////////////////////////////////////////////////////////////
    else if(req == "close_game")
    {
      endGame(connections.at(conn).current_game);
    }
    ////////////////////////////////////////////////////////////////////////////
    else if(req == "join_game")
    {
      std::string id = full.at("id");
      
      bool success = games.at(id).join(conn);
      
      if(success)
      {
        endGame(connections.at(conn).current_game);
        connections.at(conn).current_game = id;
        
        // alert both players
        connection_hdl black = games.at(id).black;
        connection_hdl white = games.at(id).white;
        
        json response = {
          {"res", "game_start"},
          {"id", id}
        };
        
        response["play_as_white"] = true;
        endpoint.send(white, response.dump(), ws_text);
        
        response["play_as_white"] = false;
        endpoint.send(black, response.dump(), ws_text);
      }
      else
      {
        sendError(conn, "Game already started.");
      }
    }
    ////////////////////////////////////////////////////////////////////////////
    else if(req == "list_games")
    {
      json list = json::array();
      
      GameMap::iterator it;
      
      for(it = games.begin(); it != games.end(); it++)
      {
        if(!it->second.priv && !it->second.started)
        {
          try
          {
            json list_entry;
            list_entry["id"] = it->first;
            bool white_in = it->second.white_in;
            list_entry["join_as_white"] = !white_in;

            connection_hdl other_player;
            if(white_in)
              other_player = it->second.white;
            else
              other_player = it->second.black;
            
            list_entry["host_name"] = connections.at(other_player).user_name;

            list_entry["starting_time"] = it->second.startingTime;
            list_entry["increment"] = it->second.increment;
            list_entry["inct"] = inctToString(it->second.inct);

            list_entry["creation_time"] = it->second.get_creation_time();

            list.push_back(list_entry);
          }
          catch(std::exception& e)
          {
            // just ignore it
          }
        }
      }
      
      json response = {
        {"res", "game_list"},
        {"list", list}
      };

      endpoint.send(conn, response.dump(), ws_text);
    }
    ////////////////////////////////////////////////////////////////////////////
    else if(req == "request_rematch")
    {
      std::string id = connections.at(conn).current_game;
      bool newGameStarted = games.at(id).rematch(conn);
      if(newGameStarted)
      {
        // alert both players
        connection_hdl black = games.at(id).black;
        connection_hdl white = games.at(id).white;
        
        json response = {
          {"res", "game_start"},
          {"id", id}
        };
        
        response["play_as_white"] = true;
        endpoint.send(white, response.dump(), ws_text);
        
        response["play_as_white"] = false;
        endpoint.send(black, response.dump(), ws_text);
      
      }
      else
      {
        json response = {
          {"res", "ack"}
        };
        
        endpoint.send(conn, response.dump(), ws_text);
      }
      
    }
    ////////////////////////////////////////////////////////////////////////////
    else if(req == "info")
    {
      connection_info ci = connections.at(conn);
      json response = {
        {"name", ci.user_name},
        {"game", ci.current_game}
      };
      endpoint.send(conn, response.dump(), ws_text);
    }
    ////////////////////////////////////////////////////////////////////////////
    else
      sendError(conn, "Unrecognized request");
  }
  catch(std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    sendError(conn, "Error generating response");
  }
}

