#include "hal2009.h"

int main (int argc, char** argv) {
    //hal2009_answer("wie gehts dem Hausmeister ?\n", argc > 1 ? argv[1] : "perl5", "de", JOIN);

    sql_engine = "sqlite";
    sql_sqlite_set_filename("test.db");
    hal2009_add_pro_file("test.pro");
    
    sleep(10000);

    return 0;
}

void hal2009_handle_signal(void* arg) {
}


