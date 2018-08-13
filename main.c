/*
	iTLS-Enso v1.5 by SKGleba
	All Rights Reserved
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include "graphics.h"

char mmit[][150] = {" -> Install 3.68 compat module"," -> Uninstall 3.68 compat module"," -> Install 3.68 root certs"," -> Exit"};
int optct = 4;

int sel = 0;
int i;
int shode = 0;

int f_copy(const char *from, const char *to) {
	long f_size;
	FILE *fp = fopen(from,"rb");

	fseek(fp, 0, SEEK_END);
	f_size = ftell(fp);
	rewind(fp);

	char* f_buffer = (char*) malloc(sizeof(char) * f_size);
	fread(f_buffer, sizeof(char), (size_t)f_size, fp);

	FILE *pFile = fopen(to, "wb");
	
	for (int i = 0; i < f_size; ++i) {
			fputc(f_buffer[i], pFile);
	}
   
	fclose(fp);
	fclose(pFile);
	return 1;
}

int f_exist(const char *fname) {
    FILE *file;
    if (file = fopen(fname, "r"))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int f_overwrite(const char *from, const char *to) {
	if (f_exist(to)) 
		sceIoRemove(to);
	long f_size;
	FILE *fp = fopen(from,"rb");

	fseek(fp, 0, SEEK_END);
	f_size = ftell(fp);
	rewind(fp);

	char* f_buffer = (char*) malloc(sizeof(char) * f_size);
	fread(f_buffer, sizeof(char), (size_t)f_size, fp);

	FILE *pFile = fopen(to, "wb");
	
	for (int i = 0; i < f_size; ++i) {
			fputc(fbuf[i], pFile);
	}
   
	fclose(fp);
	fclose(pFile);
	return 1;
}


void smenu(){
	psvDebugScreenClear(COLOR_BLACK);
	psvDebugScreenSetFgColor(COLOR_CYAN);
	psvDebugScreenPrintf("                       iTLS-Enso v1.5                            \n");
	psvDebugScreenPrintf("                         By SKGleba                              \n");
	psvDebugScreenSetFgColor(COLOR_RED);
	for(i = 0; i < optct; i++){
		if(sel==i){
			psvDebugScreenSetFgColor(COLOR_GREEN);
		}
		psvDebugScreenPrintf("%s\n", mmit[i]);
		psvDebugScreenSetFgColor(COLOR_RED);
	}
	
	psvDebugScreenSetFgColor(COLOR_GREEN);
}

int do_shit()
{
	if (sel == 3)
		sceKernelExitProcess(0);
	void *buf = malloc(0x100);
	vshIoUmount(0x300, 0, 0, 0);
	vshIoUmount(0x300, 1, 0, 0);
	_vshIoMount(0x300, 0, 2, buf);
	psvDebugScreenPrintf("Working...\n");
	if (f_exist("vs0:/data/external/webcore/ScePsp2Compat.bak") == 0)
		f_copy("vs0:/data/external/webcore/ScePsp2Compat.suprx", "vs0:/data/external/webcore/ScePsp2Compat.bak");
	switch(sel){
		case 1:
			f_overwrite("app0:Media/00", "vs0:/data/external/webcore/ScePsp2Compat.suprx");
			break;
		case 2:
			f_overwrite("vs0:/data/external/webcore/ScePspCompat.bak", "vs0:/data/external/webcore/ScePsp2Compat.suprx");
			break;
		case 3:
			f_overwrite("app0:Media/01", "vs0:/data/external/cert/CA_LIST.cer");
			break;
	}
	psvDebugScreenPrintf("Done\n");
	sceKernelDelayThread(1 * 1000 * 1000);
	sceKernelExitProcess(0);
}

int main()
{
	psvDebugScreenInit();
	psvDebugScreenClear(COLOR_BLACK);
	psvDebugScreenSetFgColor(COLOR_GREEN);
	SceCtrlData pad;
	
	int l = 1;// happens
	
	smenu();
		
	while (l == 1) {
			sceCtrlPeekBufferPositive(0, &pad, 1);

			if (pad.buttons == SCE_CTRL_CROSS) {
				do_shit();
				sel = 0;
				smenu();
				sceKernelDelayThread(0.35 * 1000 * 1000);
			}
			
			if (pad.buttons == SCE_CTRL_UP) {
				if(sel > 0){
					sel--;
				}
				smenu();
				sceKernelDelayThread(0.3 * 1000 * 1000);
			}
			
			if (pad.buttons == SCE_CTRL_DOWN) {
				if(sel + 1 < optct){
					sel++;
				}
				smenu();
				sceKernelDelayThread(0.3 * 1000 * 1000);
			}
	}
	
	sceKernelDelayThread(10 * 1000 * 1000);
	sceKernelExitProcess(0);
	return 0;
}
