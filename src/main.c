#include <stdio.h>
#include "cli-lib.h"

void cli_init(void); 
int main() {    
    cli_init();
  
    cli_print("Bem-vindo ao CLI-lib!");
  
    cli_wait_key();
  
    cli_exit();
    return 0;
}