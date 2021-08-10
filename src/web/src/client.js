var server = undefined;

function connectToServer()
{
  let ip = l("server-ip").value;
  try
  {
    server.close();
  } catch(e){}
  try
  {
    server = new WebSocket("ws://"+ip);
  }
  catch(e)
  {
    onServerError(e);
    return;
  }
  server.addEventListener('open', onServerOpen);
  server.addEventListener('message', onServerMessage);
  server.addEventListener('error', onServerError);
  server.addEventListener('close', onServerClose);
}

function disconnectFromServer()
{
  if(server == undefined || server.readyState != WebSocket.OPEN)
    return;
  server.close();
}

function onServerOpen(e)
{
  if(server == undefined || server.readyState != WebSocket.OPEN)
    return; // just in case it instantly closed
    
//  l("connection-info").classList.remove("error");
//  l("connection-info").classList.add("success");
//  l("connection-info").innerText = "Connection successful";
  
  l("connect-button").innerText = "Disconnect";
  l("connect-button").onclick = disconnectFromServer;
  
  server.send(JSON.stringify({
    req:"version",
    client_version
  }));
}

var minimum_server_version = {
  name: "standard compliant",
  major: 0,
  minor: 0,
  patch: 0
};

function verify_compatible_version(v)
{
  let mv = minimum_server_version;
  if(!v.name.endsWith(mv.name))
    return false;
      
  if(v.major < mv.major)
    return false;
  if(v.major > mv.major)
    return true;
    
  if(v.minor < mv.minor)
    return false;
  if(v.minor > mv.minor)
    return true;
    
  if(v.patch < mv.patch)
    return false;
  return true;
  
}

function onServerMessage(e)
{
  console.log(e)
  try
  {
    let response = JSON.parse(e.data);
    let res = response.res;
    
    if(res == "error")
      onServerError(response.error)
    else if(res == "conn_success")
    {
      l("connection-info").classList.remove("error");
      l("connection-info").classList.add("success");
      l("connection-info").innerText = "Successfully logged in as " + response.name;
      l("username").value = response.name;
      l("name-box").style.display = "";
    }
    else if(res == "version")
    {
      let serve_compat = verify_compatible_version(response.version);
      if(!serve_compat)
      {
        onServerError("Incompatible version");
        server.close();
      }
    }
    else if(res == "user_set")
    {
      l("connection-info").classList.remove("error");
      l("connection-info").classList.add("success");
      l("connection-info").innerText = "Successfully set username to " + response.name;
    }
    // TODO add game_created, game_closed, game_start
    
  }
  catch(error)
  {
    onServerError(error);
  }
}

function onServerError(e)
{
  let text = e;
  if(e.message)
    text = e.message;
  else if(typeof e == "object")
    text = "Failed to connect to server";
  l("connection-info").innerText = text;
  l("connection-info").classList.remove("success");
  l("connection-info").classList.add("error");
  
  if(l("connection-info").style.display == "")
    alert(text);
}

function onServerClose(e)
{
  console.log(e)
  l("connection-info").innerText += "\nConnection closed";
  if(e.reason)
    l("connection-info").innerText += ": "+e.reason;
  if(e.code == 1000)
    l("connection-info").classList.remove("error");
  else
    l("connection-info").classList.add("error");
  l("connection-info").classList.remove("success");
  //server = undefined;
  l("connect-button").innerText = "Connect";
  l("connect-button").onclick = connectToServer;
  l("name-box").style.display = "none";
}

function set_username()
{
  let name = l("username").value;
  if(!name)
    return;
  server.send(JSON.stringify({
    req:"user_set",
    name
  }));
}
