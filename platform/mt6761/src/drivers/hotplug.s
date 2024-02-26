        .text
        .global cpu_wake_up_forever_wfi
        .type   cpu_wake_up_forever_wfi, function

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ cpu_wake_up_forever_wfi
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
cpu_wake_up_forever_wfi:
1:
        isb
        dsb
        wfi
        b       1b
