#ifndef PATHRETRIEVAL_H
#define PATHRETRIEVAL_H

#define MAX_SOLUTIONS 1000000

#ifdef __cplusplus
extern "C" {
#endif

unsigned (*paretoPathsInFragment(int s_node, int d_node, const char* filename))[2];

#ifdef __cplusplus
}
#endif

#endif