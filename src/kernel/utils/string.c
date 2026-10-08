
int strlen(char *str)
{
    int len = 0;
    while (str[len] != '\0') 
    {
        len++;
    }
    return len;
}

int cntrstring(int width, char *str)
{
    return (width - strlen(str)) / 2;
}