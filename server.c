#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dqlite.h>

int main() {
  // dqlite interface to talk with client
  const char *address = "172.17.0.5:5007";
  
  // dqlite/raft directory
  const char *dir = "/tmp/dqlite-rs7";
  dqlite_node *node;
  int rv;
  mkdir(dir, 0755);
  // with default emmc (127.0.0.1 : 8889)
  // rv = dqlite_node_create(3, address, dir, &node);
  // with emmc address : .0.4:50006
  rv = dqlite_node_create_wemmc(3, address, dir, &node, "172.17.0.4", 50006); 
  if (rv != 0) {
    printf("dqlite_node_create: %d\n", rv);
  }
  rv = dqlite_node_set_bind_address(node, address);
  if (rv != 0) {
    printf("dqlite_bind_address: %d\n", rv);
  }
  rv = dqlite_node_start(node);
  if (rv != 0) {
    printf("dqlite_node_start: %d\n", rv);
  }
  //getchar();
  pause();
}
