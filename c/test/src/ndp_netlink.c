#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/socket.h>
#include "ndp_netlink.h"
#include "ndp_debug.h"

int netlink_socket_init(struct ifname_node *ifname_list)
{
    int fd = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    if(fd == -1)
    {
        ERR_LOG("create netlink socket failed:%s", strerror(errno));
        return -1;
    }

    struct sockaddr_nl sa_nl;
    memset(&sa_nl, 0, sizeof(sa_nl));
    sa_nl.nl_family = AF_NETLINK;
    sa_nl.nl_groups = RTMGRP_NEIGH;
    if(bind(fd, (struct sockaddr *) &sa_nl, sizeof(sa_nl)) == -1)
    {
        ERR_LOG("netlink bind socket failed: %s", strerror(errno));
        close(fd);
        return -1;
    }

    return 0;
}

void netlink_free_ifname(struct ifname_node *ifname_list)
{
    struct ifname_node *temp;
    while(ifname_list != NULL)
    {
        temp = ifname_list;
        ifname_list = ifname_list->next;
        free(temp);
    }
    DEBUG_LOG("ifname_list cleaned!");
}

void netlink_print_ifname(struct ifname_node *ifname_list)
{
    while(ifname_list != NULL)
    {
        fprintf(stdout, "%s\n", ifname_list->ifname);
        ifname_list = ifname_list->next;
    }
}
