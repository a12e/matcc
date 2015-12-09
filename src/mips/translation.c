#include <string.h>
#include <utility.h>
#include "translation.h"
#include "symbol_table.h"
#include "quad_list.h"
#include "register_allocation.h"

void compute_symbols_lifetime() {
    for(size_t i = 0; i < symbol_table.size; i++)
        if(symbol_table.buckets[i] != NULL)
            for(ht_bucket *bucket = symbol_table.buckets[i]; bucket != NULL; bucket = bucket->successor) {
                struct symbol *s = bucket->value;
                s->start_point = POINT_INIT;
                s->end_point = POINT_INIT;
                s->life_time = 0;
            }

    int quad_counter = 0;
    for(struct quad_list *list = quad_list; list != NULL; list = list->successor) {
        // res
        {
            if(list->quad->res->start_point == POINT_INIT)
                list->quad->res->start_point = quad_counter;
            list->quad->res->end_point = MAX(list->quad->res->end_point, quad_counter);
            list->quad->res->life_time = list->quad->res->end_point - list->quad->res->start_point;
        }

        // op1
        if(list->quad->op1) {
            if(list->quad->op1->start_point == POINT_INIT)
                list->quad->op1->start_point = quad_counter;
            list->quad->op1->end_point = MAX(list->quad->op1->end_point, quad_counter);
            list->quad->op1->life_time = list->quad->op1->end_point - list->quad->op1->start_point;
        }

        // op2
        if(list->quad->op2) {
            if(list->quad->op2->start_point == POINT_INIT)
                list->quad->op2->start_point = quad_counter;
            list->quad->op2->end_point = MAX(list->quad->op2->end_point, quad_counter);
            list->quad->op2->life_time = list->quad->op2->end_point - list->quad->op2->start_point;
        }

        quad_counter++;
    }

    for_each_symbol(bucket) {
        struct symbol *s = bucket->value;
        if(s->life_time == 0 && !s->constant) {
            fprintf(stderr, "warning: unused symbol %s\n", s->name);
        }
            }
}

void print_symbols_lifetime(FILE *f) {
    fprintf(f, "  name  start    end  total   concl.\n");
    for_each_symbol(bucket) {
        struct symbol *s = bucket->value;
        fprintf(f, "%6s %6d %6d %6d ", s->name, s->start_point, s->end_point, s->life_time);

        if(s->life_time == 0 && s->constant)
            fprintf(f, "const ok");
        else if(s->life_time == 0)
            fprintf(f, "  unused");

        fprintf(f, "\n");
            }
}

struct instruction_list *generate_code() {
    struct instruction_list *instructions = NULL;
    int quad_counter = 0;
    int instruction_counter = 0;

    // Allocate registers for each symbol
    linear_scan_register_allocation();

    for(struct quad_list *list = quad_list; list != NULL; list = list->successor) {
        switch(list->quad->op) {
            case NOP:break;
            case MOVE:break;
            case ADD:break;
            case SUB:break;
            case MUL:break;
            case DIV:break;
            case B:break;
            case BE:break;
            case BNE:break;
            case BLT:break;
            case BGT:break;
            case BLTE:break;
            case BGTE:break;
            case MAX_QUAD:break;
        }

        quad_counter++;
    }
}
