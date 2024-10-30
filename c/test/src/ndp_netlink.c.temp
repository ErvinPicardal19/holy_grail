#include <linux/neighbour.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if_ether.h>
#include <net/if.h>
#include <stdint.h>
#include "ndp_netlink.h"
#include "ndp_debug.h"

static struct ifindex_node *ifindex_list = NULL;
static int g_nl_fd = 0;

struct ifindex_node
{
    int ifindex;
    struct ifindex_node *next;
};

struct neigh_notify_hdr
{
    struct nlmsghdr nlh;
    struct ndmsg ndh;
    char rta[0];
};

int neighbor_msg_handler(struct neigh_notify_hdr *nh, ssize_t len)
{
    uint8_t l3addr[4] = {0};
    int l3addr_len = 0;
    uint8_t l2addr[ETH_ALEN] = {0};
    int l2addr_len = 0;

    struct ifindex_node *ifindex_ptr; 
    struct ndmsg *msg;

    msg = &(nh->ndh);
    ifindex_ptr = ifindex_list;

    while(ifindex_ptr)
    {
        if(ifindex_ptr->ifindex == msg->ndm_ifindex)
            goto handle;
        ifindex_ptr = ifindex_ptr->next;
    }
    
    return 0;

handle:
    switch(msg->ndm_state)
    {
        case NUD_REACHABLE:
        case NUD_STALE:
        case NUD_FAILED:
            break;
        default:
            return 0;
    }

    if(msg->ndm_type != RTN_UNICAST)
        return 0;

    unsigned int rtattr_len = NLMSG_PAYLOAD(&(nh->nlh), sizeof(nh->nlh)) - sizeof(struct ndmsg);

    struct rtattr *rta;
    for(rta = (struct rtattr *) nh->rta; RTA_OK(rta, rtattr_len); rta = RTA_NEXT(rta, rtattr_len))
    {
        int payload_len = RTA_PAYLOAD(rta);
        switch(rta->rta_type)
        {
            case NDA_DST: 
                l3addr_len = payload_len;
                memcpy(l3addr, RTA_DATA(rta), payload_len);
                break;
            case NDA_LLADDR:
                // l2addr_len = payload_len;
                memcpy(l2addr, RTA_DATA(rta), payload_len);
                break;
            default:
                 break;
        }
    }
    
    DEBUG_LOG("L3_ADDR: %02x.%02x.%02x.%02x\n", l3addr[0],l3addr[1],l3addr[2],l3addr[3]);
    DEBUG_LOG("L2_ADDR: %02x:%02x:%02x:%02x:%02x:%02x", 
              l2addr[0],l2addr[1],l2addr[2],l2addr[3],l2addr[4],l2addr[5]);

    return 0; 
}

int netlink_socket_handle(void)
{
    char buf[8192];/*8192 to avoid message truncation on platforms with page size > 4096*/
    
    ssize_t len = read(g_nl_fd, buf, sizeof(buf));
    if(len < sizeof(struct nlmsghdr))
    {
        ERR_LOG("RTM_GETNEIGH response is too small (len = %d)", len);
        return -1;
    }

    struct nlmsghdr *nh;
    for(nh = (struct nlmsghdr *) buf; NLMSG_OK(nh, len); nh = NLMSG_NEXT(nh, len))
    {
        switch(nh->nlmsg_type)
        {
            case NLMSG_DONE:
                break;
            case NLMSG_ERROR:
                ERR_LOG("Recieved NLMSG_ERROR");
                break;
            case RTM_NEWNEIGH:
            case RTM_DELNEIGH:
                neighbor_msg_handler((struct neigh_notify_hdr *) nh, len);
                break;
            default:
                break;
        }
    }

    return 0;
}

int netlink_init_neighbor(int family)
{
    struct neigh_notify_hdr req;
    ssize_t wr_bytes;

    memset(&req, 0, sizeof(req));
    req.nlh.nlmsg_len = NLMSG_LENGTH(sizeof(struct ndmsg));
    req.nlh.nlmsg_type = RTM_GETNEIGH;
    req.nlh.nlmsg_flags = (NLM_F_REQUEST | NLM_F_DUMP);
    
    req.ndh.ndm_family = family;
    req.ndh.ndm_ifindex = ifindex_list->ifindex;

    wr_bytes = write(g_nl_fd, &req, sizeof(req));
    if(wr_bytes <= 0)
    {
        ERR_LOG("failed to send RTM_GETNEIGH request: %s", strerror(errno));
        return -1;
    }

    return netlink_socket_handle();
}

void netlink_print_ifname(struct ifname_node *ifname_list)
{
    while(ifname_list != NULL)
    {
        fprintf(stdout, "%s\n", ifname_list->ifname);
        ifname_list = ifname_list->next;
    }
}

void netlink_print_ifindex(void)
{
    struct ifindex_node *temp = ifindex_list;
    while(temp != NULL)
    {
        fprintf(stdout, "%d\n", temp->ifindex);
        temp = temp->next;
    }
}

struct ifindex_node *netlink_ifindex_list_init(struct ifname_node *ifname_list)
{
    struct ifindex_node *ifindex_list, *temp, *head = NULL;
    int index;

    while(ifname_list != NULL)
    {
        if(ifname_list->ifname && *(ifname_list->ifname) != '\0')
        {
            index = if_nametoindex(ifname_list->ifname);
            if(index == 0)
            {
                ERR_LOG("Cannot get interface [%s] index.", ifname_list->ifname, strerror(errno));
                ifname_list = ifname_list->next;
                continue;
            }

            temp = (struct ifindex_node *) malloc(sizeof(struct ifindex_node));
            if(temp == NULL)
            {
                return NULL;
            }
            temp->ifindex = index;
            temp->next = NULL;

            if(head)
            {
                ifindex_list->next = temp;
                ifindex_list = ifindex_list->next;
            }
            else
            {
                ifindex_list = temp;
                head = temp;
            }
        }
        ifname_list = ifname_list->next;
    }
    
    ifindex_list = head;
    return ifindex_list;
}

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
    ifindex_list = netlink_ifindex_list_init(ifname_list);

    g_nl_fd = fd;

    return g_nl_fd;
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
