#include <geekos/screen.h>
#include <geekos/keyboard.h>
#include <geekos/string.h>
#include <geekos/user.h>
#include <geekos/syscall.h>
#include <conio.h>
static bool s_echo = true;
void Read_St(char *buf, size_t bufSize) {
    char *ptr = buf;
    size_t n = 0;
    Keycode k;
    bool done = false;
    int startrow = 0, startcol = 0;
    Get_Cursor(&startrow, &startcol);

    bufSize--;
    do {
        k = (Keycode) Get_Key();
        if ((k & KEY_SPECIAL_FLAG) || (k & KEY_RELEASE_FLAG))
            continue;

        // k &= 0xff;
        if (k == '\r')
            k = '\n';

        // Print("k:%d\n", k);


        if ((k & KEY_CTRL_FLAG) || (k & KEY_ALT_FLAG)) {
            continue;
        }

        /* discard any remaining modifiers now that we're echoing */
        k &= 0xff;

        if (s_echo&&k!='@')
            Put_Char(k);

        if (k == '@')
            done = true;

        if (n < bufSize) {
            *ptr++ = k;
            ++n;
        }
    }
    while (!done);

    *ptr = '\0';
}
int main(int argc, char **argv) {
	/*Keycode k;
	k = (Keycode) Get_Key();
	Print("k:%d\n", k);
	* */
	char buf[1024];
	Read_St(buf,1024);
	return 0;
}
