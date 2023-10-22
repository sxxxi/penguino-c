#include "headers/request_filter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <SD.h>


typedef struct ParserState {
  bool openTagFound = false;
  
  char *saveToFile = NULL;
} ParserState;


request_info *parseRequestInfo(char *raw) {
  request_info *info = (request_info *) malloc(sizeof(request_info));
  info->mapping = getAttr(raw, "<mapping>");
  info->arg = getAttr(raw, "<arg>");

  return info;
}


int compare(char *src, char *replica) {
  int offset = 0;

  while (*(replica + offset) != '\0') {
    if ( *(src + offset) != *(replica + offset) ) {
      offset = 0;
      break;
    }
    offset++;
  } 

  return offset;
}

int strSize(char *str) {
  int idx = 0;

  while (*(str + idx) != '\0') {
    idx++;
  }
  return idx;
}

char *toCStr(char *src, int size) {
  char *buf = (char *) malloc(sizeof(char) * (size + 1));
  for (int i = 0; i < size; i++) {
    buf[i] = src[i];
  }
  buf[size] = '\0';
  return buf;
}



char *getAttr(char *src, char *subj)
{
  char *start = NULL;
  char *end = NULL;

  int delimSize = strSize(subj);

  char *potential = src;
  int combo = 0;

  // Capture first
  while (*potential != '\0') {
    if (*potential == *subj) {
      int offset = compare(toCStr(potential, delimSize), subj);

      if (offset > 0) {
        potential += offset;
        start = potential;
        break;
      }
    }
    potential++;
  }

  // Capture last
  while (*potential != '\0') {
    if (*potential == *subj) {
      int offset = compare(toCStr(potential, delimSize), subj);

      if (offset > 0) {
        end = potential;
        break;
      }
    }
    potential++;
  }

  if (start == NULL || end == NULL) return NULL;

  // char *buf = (char *) malloc((end - start) * sizeof(char));

  int offset = 0;

  while (start + offset != end) {
    // buf[offset] = start[offset];
    offset++;
    // if (start + offset == end-1) {
    //   buf[offset+1] = '\0';
    // }
  }

  return toCStr(start, offset);

}













