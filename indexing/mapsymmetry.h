#include <stdint.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    unsigned char edges[12];
    unsigned char corners[8];
} MapSymmetry;

static const MapSymmetry MapRotationalSymmetries[] = {
      {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {0, 1, 2, 3, 4, 5, 6, 7}},
      {{3, 0, 1, 2, 10, 4, 9, 6, 7, 8, 11, 5}, {4, 5, 0, 1, 6, 7, 2, 3}},
      {{2, 3, 0, 1, 11, 10, 8, 9, 6, 7, 5, 4}, {6, 7, 4, 5, 2, 3, 0, 1}},
      {{1, 2, 3, 0, 5, 11, 7, 8, 9, 6, 4, 10}, {2, 3, 6, 7, 0, 1, 4, 5}},
      {{6, 9, 8, 7, 5, 4, 0, 3, 2, 1, 11, 10}, {5, 4, 7, 6, 1, 0, 3, 2}},
      {{7, 6, 9, 8, 11, 5, 1, 0, 3, 2, 10, 4}, {7, 6, 3, 2, 5, 4, 1, 0}},
      {{8, 7, 6, 9, 10, 11, 2, 1, 0, 3, 4, 5}, {3, 2, 1, 0, 7, 6, 5, 4}},
      {{9, 8, 7, 6, 4, 10, 3, 2, 1, 0, 5, 11}, {1, 0, 5, 4, 3, 2, 7, 6}},
      {{2, 11, 8, 10, 3, 1, 0, 5, 6, 4, 9, 7}, {1, 3, 0, 2, 5, 7, 4, 6}},
      {{10, 2, 11, 8, 9, 3, 4, 0, 5, 6, 7, 1}, {5, 1, 4, 0, 7, 3, 6, 2}},
      {{8, 10, 2, 11, 7, 9, 6, 4, 0, 5, 1, 3}, {7, 5, 6, 4, 3, 1, 2, 0}},
      {{11, 8, 10, 2, 1, 7, 5, 6, 4, 0, 3, 9}, {3, 7, 2, 6, 1, 5, 0, 4}},
      {{6, 5, 0, 4, 9, 7, 8, 11, 2, 10, 3, 1}, {2, 0, 3, 1, 6, 4, 7, 5}},
      {{4, 6, 5, 0, 3, 9, 10, 8, 11, 2, 1, 7}, {0, 4, 1, 5, 2, 6, 3, 7}},
      {{0, 4, 6, 5, 1, 3, 2, 10, 8, 11, 7, 9}, {4, 6, 5, 7, 0, 2, 1, 3}},
      {{5, 0, 4, 6, 7, 1, 11, 2, 10, 8, 9, 3}, {6, 2, 7, 3, 4, 0, 5, 1}},
      {{5, 7, 11, 1, 0, 6, 4, 9, 10, 3, 2, 8}, {1, 5, 3, 7, 0, 4, 2, 6}},
      {{1, 5, 7, 11, 2, 0, 3, 4, 9, 10, 8, 6}, {5, 7, 1, 3, 4, 6, 0, 2}},
      {{11, 1, 5, 7, 8, 2, 10, 3, 4, 9, 6, 0}, {7, 3, 5, 1, 6, 2, 4, 0}},
      {{7, 11, 1, 5, 6, 8, 9, 10, 3, 4, 0, 2}, {3, 1, 7, 5, 2, 0, 6, 4}},
      {{4, 3, 10, 9, 6, 0, 5, 1, 11, 7, 8, 2}, {4, 0, 6, 2, 5, 1, 7, 3}},
      {{9, 4, 3, 10, 8, 6, 7, 5, 1, 11, 2, 0}, {6, 4, 2, 0, 7, 5, 3, 1}},
      {{10, 9, 4, 3, 2, 8, 11, 7, 5, 1, 0, 6}, {2, 6, 0, 4, 3, 7, 1, 5}},
      {{3, 10, 9, 4, 0, 2, 1, 11, 7, 5, 6, 8}, {0, 2, 4, 6, 1, 3, 5, 7}}
};

static const MapSymmetry MapRotationalYSymmetries[] = {
    {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {0, 1, 2, 3, 4, 5, 6, 7}},
    {{2, 3, 0, 1, 11, 10, 8, 9, 6, 7, 5, 4}, {6, 7, 4, 5, 2, 3, 0, 1}},
    {{5, 7, 11, 1, 0, 6, 4, 9, 10, 3, 2, 8}, {1, 5, 3, 7, 0, 4, 2, 6}},
    {{11, 1, 5, 7, 8, 2, 10, 3, 4, 9, 6, 0}, {7, 3, 5, 1, 6, 2, 4, 0}},
    {{6, 9, 8, 7, 5, 4, 0, 3, 2, 1, 11, 10}, {5, 4, 7, 6, 1, 0, 3, 2}},
    {{8, 7, 6, 9, 10, 11, 2, 1, 0, 3, 4, 5}, {3, 2, 1, 0, 7, 6, 5, 4}},
    {{4, 3, 10, 9, 6, 0, 5, 1, 11, 7, 8, 2}, {4, 0, 6, 2, 5, 1, 7, 3}},
    {{10, 9, 4, 3, 2, 8, 11, 7, 5, 1, 0, 6}, {2, 6, 0, 4, 3, 7, 1, 5}}
};

uint16_t map_symmetry_apply_corners(const MapSymmetry * sym, uint16_t cornerInfo);
uint16_t map_symmetry_apply_edges(const MapSymmetry * sym, uint16_t edgesInfo);
void map_symmetry_compute_lowest(const MapSymmetry * syms, int numSym, 
                                 const uint16_t * ptrsIn, uint16_t * ptrsOut);
