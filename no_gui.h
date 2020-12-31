#ifndef NO_GUI_H
#define NO_GUI_H

#include "simulator.h"
#include "simu_functions.h"
#include "instruction.h"

#include <string.h>

#define TEXT_SIZE 50000
#define DATA_SIZE 440000
#define STACK_SIZE 60000

void exec_all(QString file_name, char* sld_file);

#endif // NO_GUI_H
