#import "headers/request_filter.h"
#import "headers/action_registry.h"
#import <stdlib.h>
#import <stdio.h>

action_registry *registry; 

int test(char *message) {
  printf(message);
}

int main() {

  char *msg = "<mapping>help<mapping><arg>Hello :)<arg>";
  registry = new_action_registry(5);
  add_entry(registry, new_registry_entry("help", test));


  request_info *info = parseRequestInfo(msg);
  action_lambda action = get(registry, info->mapping);

  if (action != NULL) {
    action(info->arg);
  } else {
    printf("Somethings wrong.");
  }

  return 0;
}
