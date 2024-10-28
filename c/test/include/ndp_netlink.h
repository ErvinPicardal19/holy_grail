#ifndef __NETLINK_H__
#define __NETLINK_H__

struct ifname_node 
{
    char *ifname;
    struct ifname_node *next;
};

struct ifindex_node
{
    int ifindex;
    struct ifindex_node *next;
};

void netlink_socket_init(struct ifname_node *ifname_list);
void netlink_free_ifname(struct ifname_node *ifname_list);
void netlink_print_ifname(struct ifname_node *ifname_list);

#endif //__NETLINK_H__
