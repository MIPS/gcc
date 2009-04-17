void Deactivate(void);
typedef struct {
    char ctype;
    char action;
    short options;
} pwdStatus_t;
void getCredentials(char *buffer)
{
    pwdStatus_t SessionSt;
    int pst = 0;
    __builtin_memset(&SessionSt,0,sizeof(SessionSt));
    __builtin_memcpy(&pst,buffer,sizeof(pwdStatus_t));
    __builtin_memcpy(&SessionSt, &pst, sizeof(pwdStatus_t));
    Deactivate();
}

