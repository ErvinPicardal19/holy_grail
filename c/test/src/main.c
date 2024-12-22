#define DEBUG 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/file.h>

#include "ndp_netlink.h"
#include "ndp_debug.h"


int main(int argc, char *argv[])
{
    struct ifname_node *ifname_list;
    int lockfd, ret = 0;
    char opt, d_flag = 0;
    int n;


    while((opt = getopt(argc, argv, "di:")) > 0)
    {
        switch(opt)
        {
            case 'i':
                if(netlink_add_ifname(optarg) == -1)
                {
                    ret = -1;
                    goto EXIT;
                }
            break;
            case 'd':
                d_flag = 1;
            default:
            break;
        }
    }

    if(argc == 1 && netlink_add_ifname("br-lan") == -1)
        goto EXIT; 

    netlink_get_ifname_list(&ifname_list);
#if DEBUG
    print_ifname_list(ifname_list); 
#endif

    lockfd = open("/tmp/ndp_lock", O_RDWR | O_CREAT, 0777);
    if(lockfd == -1)
    {
        ERR_LOG("create ndp_lock failed: %s", strerror(errno));
        ret = -1;
        goto EXIT;
    }
    if(flock(lockfd, LOCK_EX | LOCK_NB))
    {
        ERR_LOG("acquire lock failed: %s", strerror(errno));
        ret = -1;
        goto EXIT;
    }

    netlink_socket_init(ifname_list);
    
    DEBUG_LOG("ifname_list: %p\n", ifname_list);

EXIT:
    if(ifname_list)
        free_ifname_list(ifname_list);
    netlink_socket_exit();
    DEBUG_LOG("Exiting...");
    return ret;
}
