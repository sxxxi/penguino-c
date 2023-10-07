#ifndef REQUEST_FILTER_H
#define REQUEST_FILTER_H

typedef struct request_info {
    char *mapping;
    char *arg;
} request_info;

request_info *parseRequestInfo(char *raw);
int compareStrings(char *src, char *replica);
int strSize(char *str);
char *toCStr(char *src, int size);
char *getAttr(char *src, char *subj);

#endif
