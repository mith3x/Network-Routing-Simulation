#ifndef A2_FUNCTIONS_H
#define A2_FUNCTIONS_H

#include "a2_data_structures.h"

// Reads nodes from nodeinfo1.txt and creates a sorted linked list of nodes.
int get_node_info(Node **nodeList);

// Prints the list of nodes.
int print_nodes(Node *nodeList);

// Releases memory used for the nodes.
int free_nodes(Node *nodeList);

// Reads link information from linkinfo1.txt, detects any invalid nodes, prints and frees them.
int get_link_info(Node *nodeList, const char *filename);

// Reads routing table information from routeinfo1.txt and builds each node's routing table.
int get_route_info(Node *nodeList, const char *filename);

// Reads updated routing information from rerouteinfo.txt and updates the routing tables.
int get_reroute_info(Node *nodeList, const char *filename);

// Generates a set number of packets and simulates packet forwarding.
int packet_gen_and_forward(Node *nodeList, int numPackets);

// Helper function: Finds a node in the node list by name.
Node* find_node(Node *nodeList, const char *name);

#endif
