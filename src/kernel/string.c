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