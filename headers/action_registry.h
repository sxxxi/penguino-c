#ifndef ACTION_REGISTRY_H
#define ACTION_REGISTRY_H

typedef int (*action_lambda)(char *);

typedef struct action_registry {
    int size;
    int maxSize;
    char** keys;
    action_lambda *actions; 
} action_registry;

typedef struct registry_entry {
    char *key;
    action_lambda action;
} registry_entry;

void add_entry(action_registry *registry, registry_entry *entry);
action_lambda get(action_registry *registry, char *key);

action_registry* new_action_registry(int size);
registry_entry* new_registry_entry(char *key, action_lambda action);

void extend_registry(action_registry* registry, int size);
int avail_index(action_registry* registry);

#endif
