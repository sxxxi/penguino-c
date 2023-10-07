#include "headers/action_registry.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


void add_entry(action_registry *registry, registry_entry *entry)
{
    // Increase size if registry size has reached the max size
    if (registry->size == registry->maxSize) {
        extend_registry(registry, registry->maxSize * 2);
    }

    // Trust that the size is managed well.
    int emptyIdx = avail_index(registry);

    if (emptyIdx >= 0) {
      registry->keys[emptyIdx] = entry->key;
      registry->actions[emptyIdx] = entry->action;

      registry->size = registry->size + 1;
    }
}

action_lambda get(action_registry *registry, char *key)
{
    // NO TIME TO IMPLEMENT MORE EFFICIENT SEARCH ALGO
    char **keys = registry->keys;
    action_lambda* actions = registry->actions;
    action_lambda action = NULL;

    for (int i = 0; i < registry->maxSize; i++) {
        if (keys[i] != NULL && strcmp(keys[i], key) == 0) {
            action =  actions[i];
        }
    }

    return action;
}

action_registry* new_action_registry(int size)
{
    action_registry* registry = (action_registry *) calloc(1, sizeof(action_registry));
    registry -> size = 0;
    registry -> maxSize = size;
    registry -> keys = (char **) calloc(size, sizeof(char*));

    for(int i = 0; i < size; i++) {
      registry->keys[i] = NULL;
    }

    registry -> actions = (action_lambda *) malloc(sizeof(action_lambda) * size);
    return registry;
}

registry_entry* new_registry_entry(char *key, action_lambda action)
{
    registry_entry* dest = (registry_entry *) calloc(1, sizeof(registry_entry));
    dest -> key = key;
    dest -> action = action;
    return dest;
}

void extend_registry(action_registry* registry, int size) {
    if (registry -> maxSize != size) {
		registry -> maxSize = size;
        registry -> keys = (char**) realloc(registry->keys, sizeof(char*) * size);
        registry -> actions = (action_lambda*) realloc(registry->actions, sizeof(action_lambda) * size);
    } 
}

int avail_index(action_registry* registry) {
    int size = registry -> maxSize;
    char** keys = registry -> keys;
    for (int i = 0; i < size; i++) {
        if (keys[i] == NULL) {
            return i;
        }
    }
    return -1;
}
























