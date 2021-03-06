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
        if(list->quad->res) {
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

void load_symbol_in_register(struct instruction_list **instructions, struct symbol *symbol) {
    if(symbol->type == FLOAT) {
        instruction_list_push(instructions, instruction_new("l.s" , FREG_STR[symbol->affected_register], symbol->name, NULL));
    }
    else {
        instruction_list_push(instructions, instruction_new("lw" , REG_STR[symbol->affected_register], symbol->name, NULL));
    }
}

struct instruction_list *generate_code() {
    struct instruction_list *instructions = NULL;
    int quad_counter = 0;

    // Allocate normal_registers for each symbol
    linear_scan_register_allocation();

    // Put main label
    instruction_list_push(&instructions, instruction_new_label("main", "entry point"));

    for(struct quad_list *list = quad_list; list != NULL; list = list->successor) {
        char temp_str[32];
        snprintf(temp_str, 32, "L%d", quad_counter);
        instruction_list_push(&instructions, instruction_new_label(temp_str, list->quad->comment));

        // Load constants in registers for this quad
        if(list->quad->res && list->quad->res->start_point == quad_counter) {
            if(list->quad->op != B && list->quad->op != BNE && list->quad->op != BLT && list->quad->op != BGT
               && list->quad->op != BLTE && list->quad->op != BGTE && list->quad->op != PRTS)
                load_symbol_in_register(&instructions, list->quad->res);
        }
        if(list->quad->op1 && list->quad->op1->start_point == quad_counter) {
            load_symbol_in_register(&instructions, list->quad->op1);
        }
        if(list->quad->op2 && list->quad->op2->start_point == quad_counter) {
            load_symbol_in_register(&instructions, list->quad->op2);
        }

        switch(list->quad->op) {
            case NOP:
                instruction_list_push(&instructions, instruction_new("nop", NULL, NULL, NULL));
                break;
            case MOVE:
                if(list->quad->res->type == FLOAT)
                    instruction_list_push(&instructions, instruction_new(
                            "mov.s",
                            FREG_STR[list->quad->res->affected_register],
                            FREG_STR[list->quad->op1->affected_register],
                            NULL
                    ));
                else
                    instruction_list_push(&instructions, instruction_new(
                            "move",
                            REG_STR[list->quad->res->affected_register],
                            REG_STR[list->quad->op1->affected_register],
                            NULL
                    ));
                break;
            case NEG:
                if(list->quad->res->type == INT) {
                    instruction_list_push(&instructions, instruction_new(
                            "neg",
                            REG_STR[list->quad->res->affected_register],
                            REG_STR[list->quad->op1->affected_register],
                            NULL
                    ));
                }
                else if(list->quad->res->type == FLOAT) {
                    instruction_list_push(&instructions, instruction_new(
                            "neg.s",
                            FREG_STR[list->quad->res->affected_register],
                            FREG_STR[list->quad->op1->affected_register],
                            NULL
                    ));
                }
                break;
            case ADD:
            case SUB:
            case MUL:
            case DIV: {
                char *opcode;
                if(list->quad->res->type == INT) {
                    switch(list->quad->op) {
                        case ADD:opcode = "add";break;
                        case SUB:opcode = "sub";break;
                        case MUL:opcode = "mul";break;
                        case DIV:opcode = "div";break;
                        default:opcode = NULL;break;
                    }
                    instruction_list_push(&instructions, instruction_new(
                            opcode,
                            REG_STR[list->quad->res->affected_register],
                            REG_STR[list->quad->op1->affected_register],
                            REG_STR[list->quad->op2->affected_register]
                    ));
                }
                else if(list->quad->res->type == FLOAT) {
                    switch(list->quad->op) {
                        case ADD:opcode = "add.s";break;
                        case SUB:opcode = "sub.s";break;
                        case MUL:opcode = "mul.s";break;
                        case DIV:opcode = "div.s";break;
                        default:opcode = NULL;break;
                    }
                    instruction_list_push(&instructions, instruction_new(
                            opcode,
                            FREG_STR[list->quad->res->affected_register],
                            FREG_STR[list->quad->op1->affected_register],
                            FREG_STR[list->quad->op2->affected_register]
                    ));
                }
                break;
            }
            case B:
                snprintf(temp_str, 32, "L%d", list->quad->res->initial_value.intval);
                instruction_list_push(&instructions, instruction_new("b", temp_str, NULL, NULL));
                break;
            case BE:
            case BNE:
            case BLT:
            case BGT:
            case BLTE:
            case BGTE: {
                char *opcode;
                snprintf(temp_str, 32, "L%d", list->quad->res->initial_value.intval);
                if(list->quad->op1->type == INT) {
                    switch (list->quad->op) {
                        case BE:opcode = "beq";break;
                        case BNE:opcode = "bne";break;
                        case BLT:opcode = "blt";break;
                        case BGT:opcode = "bgt";break;
                        case BLTE:opcode = "ble";break;
                        case BGTE:opcode = "bge";break;
                        default:opcode = NULL;break;
                    }
                    instruction_list_push(&instructions, instruction_new(
                            opcode,
                            REG_STR[list->quad->op1->affected_register],
                            REG_STR[list->quad->op2->affected_register],
                            temp_str
                    ));
                }
                else if(list->quad->op1->type == FLOAT) {
                    switch (list->quad->op) {
                        case BE:opcode = "c.eq.s";break;
                        case BNE:opcode = "c.ne.s";break;
                        case BLT:opcode = "c.lt.s";break;
                        case BGT:opcode = "c.gt.s";break;
                        case BLTE:opcode = "c.le.s";break;
                        case BGTE:opcode = "c.ge.s";break;
                        default:opcode = NULL;break;
                    }
                    instruction_list_push(&instructions, instruction_new(
                            opcode,
                            FREG_STR[list->quad->op1->affected_register],
                            FREG_STR[list->quad->op2->affected_register],
                            temp_str
                    ));
                }
                break;
            }
            case PRTI:
                instruction_list_push(&instructions, instruction_new("li", "$v0", "1", NULL));
                instruction_list_push(&instructions, instruction_new("move", "$a0", REG_STR[list->quad->res->affected_register], NULL));
                instruction_list_push(&instructions, instruction_new("syscall", NULL, NULL, NULL));
                break;
            case PRTF:
                instruction_list_push(&instructions, instruction_new("li", "$v0", "2", NULL));
                instruction_list_push(&instructions, instruction_new("mov.s", "$f12", FREG_STR[list->quad->res->affected_register], NULL));
                instruction_list_push(&instructions, instruction_new("syscall", NULL, NULL, NULL));
                break;
            case PRTS:
                instruction_list_push(&instructions, instruction_new("li", "$v0", "4", NULL));
                instruction_list_push(&instructions, instruction_new("la", "$a0", list->quad->res->name, NULL));
                instruction_list_push(&instructions, instruction_new("syscall", NULL, NULL, NULL));
                break;
            case RET:
                instruction_list_push(&instructions, instruction_new("jal", "$ra", NULL, NULL));
                break;
            case MAX_QUAD:
                break;
        }

        quad_counter++;
    }

    return instructions;
}
