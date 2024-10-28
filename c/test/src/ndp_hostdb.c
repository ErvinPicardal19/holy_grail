#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "ndp_debug.h"
#include "ndp_hostdb.h"

static struct hostdb_node g_hostdb_host = {
    .next = &g_hostdb_host,
    .prev = &g_hostdb_host,
};

int hostdb_init(void)
{
    struct hostdb_node *host_p;
    host_p = hostdb_new_host();
    if(host_p == NULL)
    {
        ERR_LOG("hostdb_new_host failed:\t%s", strerror(errno));
        return -1;
    }
    host_p->next = g_hostdb_host.next;
    host_p->prev = &g_hostdb_host;
    g_hostdb_host.next = host_p;
    
    return 0;
}

void hostdb_free_hosts()
{
    struct hostdb_node *host_list, *temp;
    host_list = g_hostdb_host.next; 

    if(host_list == &g_hostdb_host)
        return;

    while(host_list != &g_hostdb_host)
    {
        temp = host_list;
        host_list = host_list->next;
        free(temp);
    }
    temp = NULL;

    DEBUG_LOG("host_list cleaned!");
}

struct hostdb_node *hostdb_new_host(void)
{
    struct hostdb_node *host_p;
    host_p = (struct hostdb_node *) malloc(sizeof(struct hostdb_node));
    if(host_p == NULL)
    {
        return NULL;
    }
    memset(host_p, 0, sizeof(struct hostdb_node));
    host_p->host_id = -1;

    return host_p;
}
