#include<stdio.h>
#include<pwd.h>
#include<unistd.h>

int main(void)
{
	int uid=getuid();

	struct passwd *pwd=getpwuid(uid);

	printf("uid=%d\n",uid);
	printf("name=%s\n",pwd->pw_name);
	printf("pass=%s\n",pwd->pw_passwd);

	printf("dir=%s\n",pwd->pw_dir);

	free(pwd);

	return(0);
}

