#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "signal.h"
#include "termios.h"

static void cli_disable_term_process(void)
{
	struct termios term;
	tcgetattr(fileno(stdin), &term);
	term.c_lflag &= ~ICANON; 
	term.c_lflag &= ~ECHO; 
	tcsetattr(fileno(stdin), TCSANOW, &term);
}

static void cli_enable_term_process(void)
{
	struct termios term;
	tcgetattr(fileno(stdin), &term);
	term.c_lflag |= ICANON; 
	term.c_lflag |= ECHO; 
	tcsetattr(fileno(stdin), TCSANOW, &term);
}

void sigint_handler(int sig)
{
	cli_enable_term_process();
	exit(EXIT_SUCCESS);
}

void cli_print_prompt(char *prompt)
{
	int i = 0;
	
	for (i = 0; i < strlen(prompt); i++)
	{
		putchar(prompt[i]);
	}
	putchar('>');
	putchar(' ');
}

void cli_wait_until_enter(void)
{
	while(1)
	{
		char c = 0;
		c = getchar();

		if ('\r' == c || '\n' == c)
		{
			putchar('\r');
			putchar('\n');
			break;
		}
	}
}

int cli_readline(char *linebuf, int len)
{
	int cnt = 0;

	while (1)
	{
		char c = 0;

		if (cnt >= len - 1)
		{
			cli_wait_until_enter();
			return -1;
		}

		c = getchar();

		if ('\r' == c || '\n' == c)
		{
			linebuf[cnt] = '\0';
			putchar('\r');
			putchar('\n');
			return 0;
		}
		linebuf[cnt] = c;
		cnt++;

		if (0x08 == c)
		{
			putchar('\b');
			putchar(' ');
			putchar('\b');
		}
		else
			putchar(c);
	}
}

int main(void)
{
	char buffer[16];

	memset(buffer, 0, sizeof(buffer));
	signal(SIGINT, sigint_handler);
	cli_disable_term_process();
	while (1) 
	{
#if 1
		cli_print_prompt("CONFIG");
		if (0 == cli_readline(buffer, 16))
		{
			printf("%s\n", buffer);
		}
#else
		char c = 0;
		c = getchar();
		if ('d' == c)
		{
			putchar('\b');
			putchar(' ');
		}
		else
		{
			putchar(c);
		}
		//printf("%x\n", c);
#endif		
	}
	cli_enable_term_process();

	return 0;
}
