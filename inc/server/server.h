#ifndef SERVER_H
#define SERVER_H

#include "nlohmann/json.hpp"
using nlohmann::json;

#include "version.h"
#include "game/game.h"
#include "game/clock.h"

#define ASIO_STANDALONE

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <iostream>
#include <map>
#include <string>
#include <sstream>

#include <websocketpp/common/thread.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using websocketpp::lib::thread;
using websocketpp::lib::mutex;
using websocketpp::lib::lock_guard;
using websocketpp::lib::unique_lock;
using websocketpp::lib::condition_variable;

typedef server::message_ptr message_ptr;

enum ServerAction {
  SUBSCRIBE,
  UNSUBSCRIBE,
  MESSAGE
};

struct action
{
  action(ServerAction t, connection_hdl h) : type(t), hdl(h) {}
  action(ServerAction t, connection_hdl h, server::message_ptr m)
    : type(t), hdl(h), msg(m) {}
    
  ServerAction type;
  connection_hdl hdl;
  server::message_ptr msg;
};

class Server;

struct ServerGame
{
  ServerGame(Server* _server, connection_hdl, bool priv, PlayerColor join_as,
    unsigned int startingTime,
    unsigned int increment,
    IncrementMethod inct);
  bool join(connection_hdl);
  
  bool priv;
  
  bool started;
  
  connection_hdl black;
  bool black_in;
  bool black_req_rematch;
  connection_hdl white;
  bool white_in;
  bool white_req_rematch;
  
  Game game;
  
  unsigned int startingTime;
  unsigned int increment;
  IncrementMethod inct;
  
  bool rematch(connection_hdl);
  
  unsigned int get_creation_time();

  protected:
    void start();
    Server* server;

    unsigned int creation_time;
};

struct connection_info
{
  connection_info();
  std::string user_name;
  std::string current_game;
  
  uint32_t get_id();
  
  bool operator==(const connection_info& other);
  
  private:
  uint32_t id;
  static uint32_t next_id;
};

class Server
{
  public:
    Server(json settings);
    void run(uint16_t port);
    void on_open(connection_hdl);
    void on_close(connection_hdl);
    void on_message(connection_hdl, server::message_ptr);
    void process_messages();
    
    bool verify_compatible_version(json version, connection_hdl);
    void sendError(connection_hdl, std::string, bool close = false);
    void respond(connection_hdl, std::string req, json full);
    
    static Version version;
    static Version minimum_client_version;
    
    bool conEq(connection_hdl, connection_hdl);
  private:
    typedef std::map<
        connection_hdl,
        connection_info,
        std::owner_less<connection_hdl>
      > conn_list;
    typedef std::map<
        std::string,
        ServerGame
      > GameMap;
    server endpoint;
    conn_list connections;
    std::queue<action> actions;
    
    mutex action_lock;
    mutex connection_lock;
    condition_variable action_cond;
    
    GameMap games;
    void endGame(std::string id);
    
    json settings;
};

#endif
