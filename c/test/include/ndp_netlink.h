#ifndef __NETLINK_H__
#define __NETLINK_H__

struct ifname_node 
{
    char *ifname;
    struct ifname_node *next;
};


// struct ifindex_node *netlink_ifindex_list_init(struct ifname_node *ifname_list);
int netlink_init_neighbor(int family);

int netlink_socket_init(struct ifname_node *ifname_list);
int netlink_socket_handle(void);

void netlink_free_ifname(struct ifname_node *ifname_list);
void netlink_print_ifindex(void);
void netlink_print_ifname(struct ifname_node *ifname_list);

#endif //__NETLINK_H__
