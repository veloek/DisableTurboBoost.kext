//
// DisableTurboBoost
// Copyright (C) 2020  Vegard Løkken

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <mach/mach_types.h>
#include <libkern/libkern.h>
#include <i386/proc_reg.h>

// Execute action on all CPUs
extern void mp_rendezvous_no_intrs(
        void (*action_func)(void *),
        void *arg);

const uint64_t disableTurboBoost = ((uint64_t)1) << 38;

void disable_tb(__unused void * param_not_used) {
    wrmsr64(MSR_IA32_MISC_ENABLE, rdmsr64(MSR_IA32_MISC_ENABLE) | disableTurboBoost);
}

void enable_tb(__unused void * param_not_used) {
    wrmsr64(MSR_IA32_MISC_ENABLE, rdmsr64(MSR_IA32_MISC_ENABLE) & ~disableTurboBoost);
}

kern_return_t DisableTurboBoost_start(kmod_info_t * ki, void *d)
{
    uint64_t prev = rdmsr64(MSR_IA32_MISC_ENABLE);
    mp_rendezvous_no_intrs(disable_tb, NULL);
    printf("Disabled Turbo Boost: %llx -> %llx\n", prev, rdmsr64(MSR_IA32_MISC_ENABLE));
    return KERN_SUCCESS;
}

kern_return_t DisableTurboBoost_stop(kmod_info_t *ki, void *d)
{
    uint64_t prev = rdmsr64(MSR_IA32_MISC_ENABLE);
    mp_rendezvous_no_intrs(enable_tb, NULL);
    printf("Re-enabled Turbo Boost: %llx -> %llx\n", prev, rdmsr64(MSR_IA32_MISC_ENABLE));
    return KERN_SUCCESS;
}
