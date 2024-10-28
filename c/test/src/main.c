#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/file.h>

#include "ndp_hostdb.h"
#include "ndp_netlink.h" 
#include "ndp_debug.h"


int main(int argc, char *argv[])
{
    struct ifname_node *ifname_list = NULL, *temp, *head = NULL;
    int lockfd, result = 0;
    int d_flag = 0;
    char c;
    
    while((c = getopt(argc, argv, "di:")) != -1)
    {
        switch(c)
        {

            case 'i':
                if(ifname_list)
                {
                    temp = NULL;
                    temp = (struct ifname_node *) malloc(sizeof(struct ifname_node));
                    if(temp == NULL)
                    {
                        ERR_LOG("could not get input: %s", strerror(errno));
                        break;
                    }
                    temp->ifname = optarg;
                    temp->next = NULL;
                    ifname_list->next = temp;
                    ifname_list = ifname_list->next;
                    temp = NULL;
                    break;
                } 
                else 
                {
                    temp = NULL;
                    temp = (struct ifname_node *) malloc(sizeof(struct ifname_node));
                    if(temp == NULL)
                    {
                        ERR_LOG("could not get input: %s", strerror(errno));
                        break;
                    }
                    temp->ifname = optarg;
                    temp->next = NULL;
                    ifname_list = temp;
                    head = temp;
                    temp = NULL;
                    break;
                }
            case 'd':
                d_flag = 1;
                break;
            default:
                break;
        }
        
    }

    if(ifname_list == NULL)
    {
        temp = NULL;
        temp = (struct ifname_node *) malloc(sizeof(struct ifname_node));
        if(temp == NULL)
        {
            ERR_LOG("could not get input: %s", strerror(errno));
        }
        temp->ifname = "br-lan";
        temp->next = NULL;
        ifname_list = temp;
        head = temp;
        temp = NULL;
    }
    else
    {
        ifname_list = head;
    }


    lockfd = open("/tmp/sc_neighbored_lock", O_RDWR | O_CREAT, 0777);
    if(lockfd < -1)
    {
        ERR_LOG("open/create lock file failed:\t%s", strerror(errno));
        result = -1;
        goto clean; 
    }
    if(flock(lockfd, LOCK_EX | LOCK_NB) < 0)
    {
        ERR_LOG("acquire lock failed:\t%s", strerror(errno));
        result = -1;
        goto clean;
    }

    if(d_flag && !daemon(0, 1))
    {
        DEBUG_LOG("Daemon Created!");
    }

    if(hostdb_init() == -1)
    {
        ERR_LOG("hostdb_init failed");
        hostdb_free_hosts();
        result = -1;
        goto clean; 
    }
    netlink_print_ifname(ifname_list);

clean:
    hostdb_free_hosts();
    netlink_free_ifname(ifname_list);
    return result;
}
