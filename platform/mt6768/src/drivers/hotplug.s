        .text
        .global cpu_wake_up_forever_wfi

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ cpu_wake_up_forever_wfi
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
cpu_wake_up_forever_wfi:
        .func

1:
        isb
        dsb
        wfi
        b       1b
        
        .endfunc