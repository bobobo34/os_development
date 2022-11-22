#include <kernel/interrupts.h>
#include <kernel/idt.h>
#include <kernel/panic.h>
#include <stdio.h>
EXTERN void kernel::interrupts::load_interrupts(VOID) {
    using namespace kernel::idt;
    word selector = get_cs();

    set_interrupt(0, _int00, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(1, _int01, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(2, _int02, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(3, _int03, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(4, _int04, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(5, _int05, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(6, _int06, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(7, _int07, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(8, _int08, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(9, _int09, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(10, _int10, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(11, _int11, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(12, _int12, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(13, _int13, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(14, _int14, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(16, _int16, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(17, _int17, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(18, _int18, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(19, _int19, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(20, _int20, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(21, _int21, selector, PRESENT | RING_U | INT_GATE_32);
    set_interrupt(22, 0, selector, 0);
    set_interrupt(23, 0, selector, 0);
    set_interrupt(24, 0, selector, 0);
    set_interrupt(25, 0, selector, 0);
    set_interrupt(26, 0, selector, 0);
    set_interrupt(27, 0, selector, 0);
    set_interrupt(28, 0, selector, 0);
    set_interrupt(29, 0, selector, 0);
    set_interrupt(30, 0, selector, 0);
    set_interrupt(31, 0, selector, 0);

}

using namespace kernel::interrupts;
using namespace kernel::panic;
EXTERN void int_00(VOID) {
    panic("Divide error", "#DE", false);
}
EXTERN void int_01(VOID) {
    panic("Debug error", "#DB", false);
}
EXTERN void int_02(VOID) {
    panic("NMI interrupt", "#--", false);
}
EXTERN void int_03(VOID) {
    panic("Breakpoint error", "#BP", false);
}
EXTERN void int_04(VOID) {
    panic("Overflow error", "#OF", false);
}
EXTERN void int_05(VOID) {
    panic("BOUND range exceeded", "#BR", false);
}
EXTERN void int_06(VOID) {
    panic("Invalid/undefined opcode", "#UD", false);
}
EXTERN void int_07(VOID) {
    panic("Device not available (no math coprocessor)", "#NM", false);
}
EXTERN void int_08(VOID) {
    panic("Double Fault", "#DF", true);
}
EXTERN void int_09(VOID) {
    panic("Coprocessor segment overrun", "", false);
}
EXTERN void int_10(VOID) {
    panic("Invalid TSS", "#TS", false);
}
EXTERN void int_11(VOID) {
    panic("Segment not present", "#NP", false);
}
EXTERN void int_12(VOID) {
    panic("Stack-segment fault", "#SS", false);
}
EXTERN void int_13(VOID) {
    panic("General protection", "#GP", false);
}
EXTERN void int_14(uint32_t cr2) {
    panic("Page fault", "#PF", true);
    printf("Tried to access address %X", cr2);
}
EXTERN void int_16(VOID) {
    panic("Math fault", "#MF", false);
}
EXTERN void int_17(VOID) {
    panic("Alignment Check", "#AC", false);
}
EXTERN void int_18(VOID) {
    panic("Machine Check", "#MC", true);
}
EXTERN void int_19(VOID) {
    panic("SIMD floating-point exception", "#XM", false);
}
EXTERN void int_20(VOID) {
    panic("Virtualization Exception", "#VE", false);
}
EXTERN void int_21(VOID) {
    panic("Control Protection Exception", "#CP", false);
}








