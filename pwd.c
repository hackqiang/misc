/*
 *       			密码加密
 *
 *	编译： cc pwd.c -lcrypt
 * 	usage:
 *		./a.out passwd
 *
 *
 */





#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <shadow.h>


#define SHADOW_PWD
int checkpasswd(const char *username, const char *passwd)
{

	struct passwd *pwd=NULL;	
	char *salt=NULL;
	
	if ((pwd = getpwnam(username))) {
#  ifdef SHADOW_PWD
	    struct spwd *sp;
	    if ((sp = getspnam(username)))
	      pwd->pw_passwd = sp->sp_pwdp;
#  endif
	    salt = pwd->pw_passwd;
	} else
	  salt = "xx";	
	
	//printf("read from /etc/shadow passwd:\t%s\n",pwd->pw_passwd);
	//printf("crypted from %s passwd:\t%s\n",passwd,crypt(passwd, salt));
	
	if (!strncmp(crypt(passwd, salt),pwd->pw_passwd,strlen(pwd->pw_passwd))) {
		return 0;
	}

	return -1;
}


int main(int argc, char **argv)
{
	int ret=checkpasswd(argv[1],argv[2]);
	if (!ret)
		printf("success\n");
	else
		printf("wrong passwd\n");
	
	return 0;
}
