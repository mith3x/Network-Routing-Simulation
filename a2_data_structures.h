#ifndef A2_DATA_STRUCTURES_H
#define A2_DATA_STRUCTURES_H

#define MAX_NAME_LEN 50
#define MAX_IP_LEN 16

// Structure for a network node.
typedef struct Node {
    char name[MAX_NAME_LEN];
    char ip[MAX_IP_LEN];
    struct RouteEntry *routingTable; // pointer to linked list of routing entries
    struct Node *next;
} Node;

// Structure for a routing table entry.
typedef struct RouteEntry {
    char destination[MAX_NAME_LEN];
    char nextHop[MAX_NAME_LEN];
    int cost;
    struct RouteEntry *next;
} RouteEntry;

// Structure for an invalid node.
typedef struct InvalidNode {
    char name[MAX_NAME_LEN];
    struct InvalidNode *next;
} InvalidNode;

// Structure for a network link.
typedef struct Link {
    char node1[MAX_NAME_LEN];
    char node2[MAX_NAME_LEN];
    int cost;
    int bandwidth;
} Link;

// Structure for a data packet.
typedef struct Packet {
    char src[MAX_NAME_LEN];
    char dst[MAX_NAME_LEN];
    int size;
} Packet;

#endif
