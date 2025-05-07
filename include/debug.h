//
// Created by Sedrak Dalaloyan on 12/22/24.
//

#ifndef CODECRAFTERS_SHELL_C_DEBUG_H
#define CODECRAFTERS_SHELL_C_DEBUG_H

#ifndef DEBUG_H
#define DEBUG_H

/* -- Macro Definitions */

#ifdef DEBUG
#define TRACE(x)    db_print x
#else
#define TRACE(x)
#endif /* DEBUG */

/* -- Declarations */

#ifdef DEBUG
extern  int     debug;
#endif

#endif  /* DEBUG_H */

#endif //CODECRAFTERS_SHELL_C_DEBUG_H
