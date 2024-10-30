#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "ndp_netlink.h"
#include "ndp_debug.h"

static struct ifname_node *ifname_list = NULL;

void netlink_get_ifname_list(struct ifname_node **list)
{
    *list = ifname_list;
}

int netlink_add_ifname(char *name)
{
    DEBUG_LOG("HERE! %s", name);
    static struct ifname_node *head = NULL, *tail = NULL;
    struct ifname_node *tmp;

    tmp = (struct ifname_node *) malloc(sizeof(struct ifname_node));
    if(!tmp)
    {
        ERR_LOG("Create ifname_node failed: %s", strerror(errno));
        return -1;
    }
    tmp->name = name;
    tmp->next = NULL;

    if(head)
    {
        tail->next = tmp;
        tail = tail->next;
        tmp = NULL;
    }
    else
    {
        head = tmp;
        tail = tmp;
        ifname_list = head;
        tmp = NULL;
    }

    return 0;
}

void print_ifname_list(struct ifname_node *ifname_list)
{
    printf("--IFNAME_LIST--\n");
    while(ifname_list) 
    {
        printf("%s\n", ifname_list->name);
        ifname_list = ifname_list->next;
    }
}

void free_ifname_list(struct ifname_node *ifname_list)
{
    struct ifname_node *tmp = ifname_list;

    while(ifname_list)
    {
        ifname_list = ifname_list->next;
        free(tmp);
        tmp = ifname_list;
    }

    // DEBUG_LOG("Done.");
}
