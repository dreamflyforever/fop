#include <stdlib.h>
#include <stdio.h>

char *itoa(int num,char *str,int radix)
{
	char *retvalue;
	if (str == NULL) {
		retvalue = NULL;
		goto
			end;
	}

	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
	unsigned unum;
	int i = 0, j, k; 
	if ((radix == 10) && (num < 0)) {
		unum=(unsigned)-num; 
		str[i++]='-'; 
	}  else {
		unum = (unsigned)num;
	}

	do { 
		str[i++] = index[unum % (unsigned)radix]; 
		unum /= radix; 
	} while(unum); 
	str[i]='\0'; 
	if (str[0]=='-') k=1;
	else k=0; 
	char temp; 
	for (j = k; j <= (i-k-1)/2.0; j++) { 
		temp=str[j]; 
		str[j] = str[i-j-1]; 
		str[i-j-1] = temp; 
	}
	retvalue = str;
end:
	return retvalue; 
}

#if 0
int main()
{
	int num = 0;
	char str[25];
	int i;
	for (i = 0; i < 100; i++) {
		itoa(num, str, 10);
		printf("%s\n", str);
		num++;
	}
	return 0;
}
#endif
