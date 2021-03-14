int strlen(char *string)
{
    char *a = string;
    int ans = 0;
    while (*a != 0x0)
    {
        a++;
        ans += 1;
    }
    return ans;
}

int strcmp(char *first, char *second)
{
    if (strlen(first) != strlen(second))
    {
        return 0;
    }
    else
    {
        int idx = 0;
        char *temp = first;
        while (*temp != 0x00)
        {
            if (first[idx] != second[idx])
            {
                return 0;
            }
            temp++;
            idx++;
        }
        return 1;
    }
}
char strswith(char *first, char *second, int length)
{
    int i;

    for (i = 0; i < length; i++)
    {
        if (second[i] == 0)
        {
            return 1;
        }
        if (first[i] != second[i])
        {
            return 0;
        }
    }

    return 1;
}

// int streq(char *first, char *second){
// 	int i;
//     if (strlen(first) != strlen(second)){
//         return 0;
//     }
//     int l;
//     l = strlen(first);
// 	for (i = 0; i < l; ++i) {
// 		if (first[i] != second[i]) {
// 			return 0;
// 		}
// 	}
// 	return 1;
// }