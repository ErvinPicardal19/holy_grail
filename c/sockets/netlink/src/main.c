#include <errno.h>
#include <linux/neighbour.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/syslog.h>
#include <unistd.h>

#include "debug_utils.h"

struct notify_neigh_request
{
    struct nlmsghdr nl_hdr;
    struct ndmsg nd_hdr;
    char rtattr[0];
} __attribute__((__packed__));

void handle_msg(struct notify_neigh_request *nhr, unsigned long len)
{
    struct nlmsghdr *nlm_hdr = (struct nlmsghdr *)nhr;
    struct ndmsg *msg = (struct ndmsg *)(((char *)nhr) + sizeof(struct nlmsghdr));
    uint8_t l3addr[4];
    unsigned l3addr_len;
    uint8_t l2addr[6];
    char l2addr_len;

    switch (msg->ndm_state) {
    case NUD_REACHABLE: DEBUG_LOG("NUD_REACHABLE"); break;
    case NUD_STALE: DEBUG_LOG("NUD_STALE"); break;
    case NUD_FAILED: DEBUG_LOG("NUD_FAILED"); break;
    default: return;
    }

    if (msg->ndm_type != RTN_UNICAST) return;

    unsigned long rtattr_len =
        NLMSG_PAYLOAD(nlm_hdr, sizeof(struct nlmsghdr)) - sizeof(struct ndmsg);
    struct rtattr *rta;

    memset(l3addr, 0, sizeof(l3addr));
    memset(l2addr, 0, sizeof(l2addr));
    for (rta = (struct rtattr *)nhr->rtattr; RTA_OK(rta, rtattr_len);
         rta = RTA_NEXT(rta, rtattr_len)) {
        int payload_len = RTA_PAYLOAD(rta);
        switch (rta->rta_type) {
        case NDA_DST:
            l3addr_len = payload_len;
            memcpy(l3addr, RTA_DATA(rta), payload_len);
            hexdump(l3addr, payload_len);
            break;
        case NDA_LLADDR:
            l2addr_len = payload_len;
            memcpy(l2addr, RTA_DATA(rta), payload_len);
            hexdump(l2addr, payload_len);
            break;
        default: break;
        }
    }

    DEBUG_LOG("L2: %02x:%02x:%02x:%02x:%02x:%02x", l2addr[0], l2addr[1], l2addr[2],
              l2addr[3], l2addr[4], l2addr[5]);
}

int main(void)
{
    struct notify_neigh_request nhr;
    struct sockaddr_nl sa;
    char buf[8192];
    int nl_fd = 0;

    nl_fd = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    if (nl_fd == -1) {
        ERROR_LOG("Create netlink socket failed: %s", strerror(errno));
        goto EXIT;
    }

    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_groups = RTMGRP_NEIGH;
    if (bind(nl_fd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
        ERROR_LOG("Cannot bind netlink socket: %s", strerror(errno));
        goto EXIT;
    }

    memset(&nhr, 0, sizeof(nhr));
    nhr.nl_hdr.nlmsg_len = NLMSG_LENGTH(sizeof(struct ndmsg));
    nhr.nl_hdr.nlmsg_type = RTM_GETNEIGH;
    nhr.nl_hdr.nlmsg_flags = (NLM_F_REQUEST | NLM_F_DUMP);

    nhr.nd_hdr.ndm_family = AF_INET;
    nhr.nd_hdr.ndm_ifindex = if_nametoindex("wlp2s0");

    int wr_bytes = write(nl_fd, &nhr, sizeof(nhr));
    if (wr_bytes <= 0) {
        ERROR_LOG("Send RTM_GETNEIGH request failed: %s", strerror(errno));
        goto EXIT;
    }

    memset(buf, 0, sizeof(buf));
    ssize_t len = read(nl_fd, buf, sizeof(buf));
    if (len < sizeof(struct nlmsghdr)) {
        ERROR_LOG("Reply from RTM_GETNEIGH request is too small");
        goto EXIT;
    }

    // hexdump(buf, sizeof(len));

    struct nlmsghdr *nlm = (struct nlmsghdr *)buf;
    if (!NLMSG_OK(nlm, len)) {
        DEBUG_LOG("NLMSG header is malformed");
        goto EXIT;
    }

    switch (nlm->nlmsg_type) {
    case NLMSG_DONE: break;
    case NLMSG_ERROR: ERROR_LOG("Error on returned NLMSG type"); break;
    case RTM_NEWNEIGH:
    case RTM_DELNEIGH: handle_msg((struct notify_neigh_request *)nlm, len); break;
    default: break;
    }

EXIT:
    if (nl_fd > 0) close(nl_fd);
    return 0;
}
