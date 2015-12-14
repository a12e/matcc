#ifndef MATCC_REGISTER_ALLOCATION_H
#define MATCC_REGISTER_ALLOCATION_H

#define MAX_REGISTERS 8
extern char *REG_STR[MAX_REGISTERS];
#define MAX_FREGISTERS 16
extern char *FREG_STR[MAX_FREGISTERS];

void linear_scan_register_allocation();

#endif //MATCC_REGISTER_ALLOCATION_H
