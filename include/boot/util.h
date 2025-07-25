#pragma once

#include <uefi.h>

extern int ctr;
void EFIAPI TimerCallback(efi_event_t Event, void* Context);