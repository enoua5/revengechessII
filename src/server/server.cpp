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
Version Server::version = Version("Revenge Chess Server, standard compliant", 'x', 0, 0, 3);
// change the version name here if the server should reject the vanilla client
Version Server::minimum_client_version = Version("standard compliant", 'x', 2, 2, 3);

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

      server::connection_ptr con = endpoint.get_con_from_hdl(a.hdl);
      std::string path = con->get_resource();
      // probably only need to check the "/" case, but I don't trust like that
      if(path != "" && path != "/")
      {
        std::size_t last_slash = path.find_last_of('/');
        std::string id = path.substr(last_slash+1);
        json spoof = {
          {"id", id}
        };
        respond(a.hdl, "join_game", spoof);
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
    black_in = false;
  }
  else
  {
    black = hdl;
    white_in = false;
    black_in = true;
  }

  creation_time = time_to_mill(get_current_time());
}

class AlreadyInGameError : std::exception{};

bool ServerGame::join(connection_hdl hdl)
{

  bool joined = false;
  if(!white_in)
  {
    if(server->conEq(hdl, black))
      throw AlreadyInGameError();
  
    white = hdl;
    white_in = true;
    joined = true;
  }
  
  else if(!black_in)
  {
    if(server->conEq(hdl, white))
      throw AlreadyInGameError();

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

  game.startClock();
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
  try
  {
    return connections.at(a) == connections.at(b);
  }
  catch(const std::exception& e){}
  return false;
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
      
      bool success;
      std::string errorReason = "Game already started";
      try
      {
        if(games.count(id))
          success = games.at(id).join(conn);
        else
        {
          success = false;
          errorReason = "Could not find game";
        }
      }
      catch(const AlreadyInGameError& e)
      {
        success = false;
        errorReason = "You're already in that game!";
      }
      
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
        sendError(conn, errorReason);
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
            std::cerr << "error adding game to list in list_games." << std::endl;
            std::cerr << e.what() << std::endl;
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
    else if(req == "get_board")
    {
      connection_info ci = connections.at(conn);
      if(games.count(ci.current_game))
      {
        Game g = games.at(ci.current_game).game;
        std::string board = g.board.serialize();

        json white_timer = {
          {"time_left", g.clock.getWhiteTime()},
          {"increment", g.clock.getWhiteIncrementAmount()},
          {"inct", inctToString(g.clock.getWhiteIncType())}
        };

        json black_timer = {
          {"time_left", g.clock.getBlackTime()},
          {"increment", g.clock.getBlackIncrementAmount()},
          {"inct", inctToString(g.clock.getBlackIncType())}
        };


        json response = {
          {"res", "board_state"},
          {"board_str", board},
          {"white_timer", white_timer},
          {"black_timer", black_timer}
        };
        endpoint.send(conn, response.dump(), ws_text);
      }
    }
    else if(req == "make_move")
    {
      connection_info ci = connections.at(conn);
      if(games.count(ci.current_game))
      {
        ServerGame sg = games.at(ci.current_game);
        connection_hdl conn_turn = sg.black;
        if(sg.game.board.turn == WHITE)
          conn_turn = sg.white;
        if(!conEq(conn, conn_turn))
        {
          sendError(conn, "Not your turn! (how did you get here?)");
          return;
        }

        unsigned char from = full.at("from");
        unsigned char to = full.at("to");
        std::string promo_str = full.at("promo");
        PieceType promo = NO;
        if(promo_str == "PAWN")
          promo = PAWN;
        else if(promo_str == "ROOK")
          promo = ROOK;
        else if(promo_str == "KNIGHT")
          promo = KNIGHT;
        else if(promo_str == "BISHOP")
          promo = BISHOP;
        else if(promo_str == "KING")
          promo = KING;
        else if(promo_str == "QUEEN")
          promo = QUEEN;

        Move move = Move(Square(from), Square(to), promo);
        try
        {
          games.at(ci.current_game).game.commitMove(move);
        }
        catch(const std::exception& e)
        {
          sendError(conn, "Not a legal move! (how did you get here?)");
        }
        
      }
    }
    ////////////////////////////////////////////////////////////////////////////
    else
      sendError(conn, "Unrecognized request");
  }
  catch(std::exception& e)
  {
    std::cerr << "Issuing generic error because of:" << std::endl;
    std::cerr << e.what() << std::endl;
    sendError(conn, "Error generating response");
  }
}

