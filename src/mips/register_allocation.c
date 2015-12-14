#include <utility.h>
#include <string.h>
#include <symbol_table.h>
#include "register_allocation.h"
#include "translation.h"

extern bool debug;

struct symbol_array {
    struct symbol **symbols;
    size_t size;
} live_symbols, active_symbols;

void symbol_array_push(struct symbol_array *array, struct symbol *s) {
    array->size++;
    array->symbols = safe_realloc(array->symbols, array->size * sizeof(struct symbol *));
    array->symbols[array->size - 1] = s;
}

void symbol_array_remove(struct symbol_array *array, size_t index) {
    if(index != array->size - 1)
        memmove(&array->symbols[index], &array->symbols[index + 1],
                (array->size - index - 1) * sizeof(struct symbol *));
    array->size--;
    array->symbols = safe_realloc(array->symbols, array->size * sizeof(struct symbol *));
}

void symbol_array_free(struct symbol_array *array) {
    safe_free(array->symbols);
    array->size = 0;
}

int symbols_sort_inc_start_fn(const void *s1, const void *s2) {
    return (*(const struct symbol **) s1)->start_point - (*(const struct symbol **) s2)->start_point;
}

int symbols_sort_inc_end_fn(const void *s1, const void *s2) {
    return (*(const struct symbol **) s1)->end_point - (*(const struct symbol **) s2)->end_point;
}

void generate_live_symbols() {
    memset(&live_symbols, 0, sizeof(live_symbols));
    for_each_symbol(bucket)
        symbol_array_push(&live_symbols, bucket->value);
    for_each_symbol(bucket)
        bucket->value->affected_register = REGISTER_NONE;
    qsort(live_symbols.symbols, live_symbols.size, sizeof(struct symbol *), &symbols_sort_inc_start_fn);
}

void sort_active_symbols() {
    qsort(active_symbols.symbols, active_symbols.size, sizeof(struct symbol *), &symbols_sort_inc_end_fn);
}

char *REGSTR[MAX_REGISTERS] = {
        "$t0", "$t1", "$t2", "$t3",
        "$t4", "$t5", "$t6", "$t7",
};

struct {
    struct symbol *symbol;  // The symbol living in this register, NULL if the register is free
    bool dirty;             // Whether the value of the symbol in this register needs to be reuploaded to RAM
} registers[MAX_REGISTERS];

int next_free_register() {
    for(int i = 0; i < MAX_REGISTERS; i++)
        if(registers[i].symbol == NULL)
            return i;
    fprintf(stderr, "error: no more free register (why no spilling ?)\n");
    return REGISTER_NONE;
}

void assign_register(int reg, struct symbol *s) {
    if(registers[reg].symbol != NULL) {
        fprintf(stderr, "error: trying to assign to %s already assigned register %s to %s\n",
                s->name, REGSTR[reg], registers[reg].symbol->name);
    }
    else {
        registers[reg].symbol = s;
        s->affected_register = reg;
        if(debug) fprintf(stderr, "debug: assigning register %s to symbol %s\n",
                REGSTR[reg], registers[reg].symbol->name);
    }
}

void free_register(int reg) {
    if(registers[reg].symbol == NULL) {
        fprintf(stderr, "error: trying to free already free register %s\n", REGSTR[reg]);
    }
    else {
        if(debug) fprintf(stderr, "debug: freeing register %s of symbol %s\n",
                REGSTR[reg], registers[reg].symbol->name);
        registers[reg].symbol = NULL;
    }
}

int current_stack_location = 0;
int new_stack_location(size_t symbol_size) {
    current_stack_location -= symbol_size;
    return current_stack_location;
}

void expire_old_symbols(struct symbol *symbol) {
    for(size_t j = 0; j < active_symbols.size; j++) {
        if(active_symbols.symbols[j]->end_point >= symbol->start_point)
            return;
        // add register[j] to pool of free registers
        free_register(active_symbols.symbols[j]->affected_register);
        // remove j from active
        symbol_array_remove(&active_symbols, j);
        sort_active_symbols();
    }
}

void spill_symbol(struct symbol *symbol) {
    if(active_symbols.size == 0) {
        fprintf(stderr, "error: trying to spill symbol %s that is not active\n", symbol->name);
    }
    else {
        if(debug) fprintf(stderr, "debug: spilling symbol %s\n", symbol->name);
        struct symbol *spill = active_symbols.symbols[active_symbols.size - 1];

        if(symbol->end_point < spill->end_point) {
            symbol->affected_register = spill->affected_register;
            spill->stack_location = new_stack_location(spill->size);
            // remove spill from active
            symbol_array_remove(&active_symbols, active_symbols.size - 1);
            // add symbol to the active array
            symbol_array_push(&active_symbols, symbol);
            sort_active_symbols();
        }
        else {
            symbol->stack_location = new_stack_location(symbol->size);
        }
    }
}

void linear_scan_register_allocation() {
    // Statistics
    size_t max_assigned_registers = 0;
    // Initialize registers
    memset(&registers, 0, sizeof(registers));
    // Generate live symbols
    generate_live_symbols();
    // Initialize active symbols
    memset(&active_symbols, 0, sizeof(active_symbols));

    if(debug) fprintf(stderr, "debug: %zu total symbols: { ", live_symbols.size);
    for(int j = 0; j < live_symbols.size; j++)
        if(debug) fprintf(stderr, "%s(start=%d) ", live_symbols.symbols[j]->name, live_symbols.symbols[j]->start_point);
    if(debug) fprintf(stderr, "}\n");

    // Assign register to every live symbol
    for(int i = 0; i < live_symbols.size; i++) {
        expire_old_symbols(live_symbols.symbols[i]);

        // Too many symbols active (<==> registers assigned)
        if(active_symbols.size > MAX_REGISTERS) {
            // Need to put symbols on the stack
            spill_symbol(live_symbols.symbols[i]);
        }
        else {
            // There is free registers, so simply assign one
            assign_register(next_free_register(), live_symbols.symbols[i]);
            // Now this symbol is active
            symbol_array_push(&active_symbols, live_symbols.symbols[i]);
            sort_active_symbols();
        }

        if(debug) fprintf(stderr, "debug: %zu active symbols: { ", active_symbols.size);
        for(int j = 0; j < active_symbols.size; j++)
            if(debug) fprintf(stderr, "%s(end=%d) ", active_symbols.symbols[j]->name, active_symbols.symbols[j]->end_point);
        if(debug) fprintf(stderr, "}\n");

        max_assigned_registers = MAX(max_assigned_registers, active_symbols.size);
    }

    if(debug) fprintf(stderr, "max_assigned_registers = %zu\n", max_assigned_registers);

    symbol_array_free(&live_symbols);
    symbol_array_free(&active_symbols);
}