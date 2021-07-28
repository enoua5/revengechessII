#include "server/server.h"

connection_info::connection_info()
{
  user_name = "";
  logged_in = false;
}

Server::Server()
{
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
    }
    else if(a.type == UNSUBSCRIBE)
    {
      lock_guard<mutex> guard(connection_lock);
      connections.erase(a.hdl);
    }
    else if(a.type == MESSAGE)
    {
      lock_guard<mutex> guard(connection_lock);
    }
  }
}

