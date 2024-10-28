#ifndef __HOSTDB_INIT_H__
#define __HOSTDB_INIT_H__

#include <netdb.h>

struct hostdb_node
{
    char name[NI_MAXHOST]; // Accepts both FQDNs and Local System Hostname sizes
    int sync;
    int host_id;
    struct hostdb_node *next;
    struct hostdb_node *prev;
};

int hostdb_init(void);
struct hostdb_node *hostdb_new_host(void);
void hostdb_free_hosts();

#endif //__HOSTDB_INIT_H__
