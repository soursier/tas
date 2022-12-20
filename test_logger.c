#include "logger.h"

int main()
{
    logger_init_log(LOGGING);
    int cinq = 5;
    logger_log("dfsgdsfg", 0, &cinq);
    logger_free();

    logger_init_log(READING);
    logger_read();
    logger_free();
    return 0;
}