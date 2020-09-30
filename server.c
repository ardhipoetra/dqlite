#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dqlite.h>

int main() {
  const char *address = "172.17.0.5:5007";
  const char *dir = "/tmp/dqlite-rs7";
  dqlite_node *node;
  int rv;
  mkdir(dir, 0755);
  rv = dqlite_node_create(3, address, dir, &node);
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
