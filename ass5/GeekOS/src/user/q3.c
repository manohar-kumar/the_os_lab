#include <geekos/screen.h>
#include <geekos/keyboard.h>
#include <geekos/string.h>
#include <geekos/user.h>
#include <geekos/syscall.h>
#include <conio.h>
#include <sched.h>
static bool s_echo = true;

int main(int argc, char **argv) {
	/*Keycode k;
	k = (Keycode) Get_Key();
	Print("k:%d\n", k);
	* */
	int xyz,a;
	a=Get_Time_Of_Day();
	Get_NewTOD(&xyz);
	Get_Nosyscalls();
	Print("%d",xyz);
	Print("%d",a);
	return 0;
}
