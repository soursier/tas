#ifndef LOGGER_H
#define LOGGER_H
typedef enum{NOT_INITIALIZED,READING,LOGGING}logger_state_t;
int logger_init_log(logger_state_t mode);
int logger_log(char* operation, int size_byte,void* address);
int logger_free();
int logger_read();
#endif /*LOGGER_H*/