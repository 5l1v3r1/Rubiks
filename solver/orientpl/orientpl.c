#include "orientpl.h"

void orientpl_initialize(OrientPl * pl) {
    pl->operations = cube_standard_face_turns();
}

void orientpl_free(OrientPl pl) {
    int i;
    for (i = 0; i < 18; i++) {
        rubiks_map_free(pl.operations[i]);
    }
    free(pl.operations);
}

void * orientpl_create_group_identity(void * data) {
    return rubiks_map_new_identity();
}

void orientpl_free_group_object(void * data, void * object) {
    rubiks_map_free((RubiksMap *)object);
}

void orientpl_operate(void * data, void * destination, void * object, int operation) {
    OrientPl * pl = (OrientPl *)data;
    rubiks_map_multiply(destination, pl->operations[operation], object);
}

int orientpl_is_goal(void * data, void * object) {
    uint16_t values = rubiks_map_edge_orientations((RubiksMap *)object);
    return values == 0xfff;
}

int orientpl_accepts_sequence(void * data, unsigned char * moves, int count) {
    return standard_operations_validate(moves, count);
}

int orientpl_heuristic_exceeds(void * data, void * object, int maxMoves) {
    OrientPl * pl = (OrientPl *)data;
    if (!pl->edgeHeuristic) {
        return 0;
    }
    int count = heuristic_table_lookup_map(pl->edgeHeuristic, (RubiksMap *)object);
    return (count > maxMoves);
}

void orientpl_report_solution(void * data, unsigned char * moves, int count) {
    printf("Found solution: ");
    cube_print_standard_solution(moves, count);
    printf("\n");
}

void orientpl_report_progress(void * data, time_t duration, long long expandedCount, int depth) {
    printf("Expanded %lld nodes [depth = %d, time = %lld seconds]\n", 
           expandedCount, depth, (long long)duration);
}