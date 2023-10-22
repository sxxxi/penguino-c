#include <map>
#include <string>
#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

typedef void (*RequestHandler)(std::string);

class HandlerRegistry {
private:
  std::map<std::string, RequestHandler> handlers;

public:
  void addHandler(std::string mapping, RequestHandler handler) 
  {
    handlers[mapping] = handler;
  }

  RequestHandler getHandler(std::string mapping) 
  {
    return handlers[mapping];
  }
};


#endif