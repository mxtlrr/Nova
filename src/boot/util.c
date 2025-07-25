#include "util.h"

int ctr = 0;
void EFIAPI TimerCallback(efi_event_t Event, void* Context){
	ctr++;
}
