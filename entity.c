#include "entity.h"
#include <stdbool.h>

const int starting_entity_array_size = 10;

bool inited = false;

entity_prototype entity_prototypes[NUM_ET];

entity_prototype entity_prototype_get(entity_type et) {
    if (!inited) {
        entity_prototypes[ET_PLAYER] = (entity_prototype){"player", "pP", {64, 0, 16, 16}, true, 0};
        entity_prototypes[ET_CRATE] = (entity_prototype){"crate", "cC", {96, 0, 16, 16}, true, 1};
        entity_prototypes[ET_PRESENT] = (entity_prototype){"present", "bB", {48, 0, 16, 16}, true, 1};
        entity_prototypes[ET_TARGET] = (entity_prototype){"target", "tT", {80, 0, 16, 16}, false, 0};
        entity_prototypes[ET_NONE] = (entity_prototype){"none", "", {0, 0, 0, 0}, false, 0};
    }
    inited = true;

    return entity_prototypes[et];
}

void entity_vla_append(entity_vla *v, entity e) {
    if (v->num_entities == v->backing_array_size - 1) {
        v->backing_array_size *= 2;
        v->entities = (entity *)realloc(v->entities, sizeof(entity) * v->backing_array_size);
        for (int i = v->backing_array_size / 2; i < v->backing_array_size; i++) {
            v->entities[i].et = ET_NONE;
        }
    }
    v->entities[v->num_entities++] = e;
}

entity_vla entity_vla_clone(entity_vla *v) {
    entity_vla v2 = {0};
    v2.backing_array_size = v->backing_array_size;
    v2.num_entities = v->num_entities;
    v2.entities = (entity *)malloc(v->backing_array_size * sizeof(entity));
    memset(v2.entities, 0, sizeof(entity) * v2.backing_array_size);
    memmove(v2.entities, v->entities, sizeof(entity) * v->num_entities);
    return v2;
}

void entity_vla_destroy(entity_vla *v) {
    if (v->entities) {
        free(v->entities);
        v->entities = 0;
    }
    v->backing_array_size = 0;
    v->num_entities = 0;
}

void entity_vla_init(entity_vla *v) {
    v->entities = (entity*)calloc(starting_entity_array_size, sizeof(entity));
    v->num_entities = 0;
    v->backing_array_size = starting_entity_array_size;
}

void entity_vla_print(entity_vla *v) {
    printf("entity vla len %d bs %d\n", v->num_entities, v->backing_array_size);
    for (int i = 0; i < v->num_entities; i++) {
        printf("\t %s %d %d\n", entity_prototype_get(v->entities[i].et).name, v->entities[i].x, v->entities[i].y);
    }
}

// todo make entity prototypes a function so its not fucking annoying to use hopefully
// and then get a big print out of the stack to get a idea of wtf its doing
// then hopefully it will undo correctly
// then turn back on the level free or whatever and see why its double freing or have fixed it