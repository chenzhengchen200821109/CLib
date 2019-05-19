/*
 * regs[0]: ret
 * regs[1]: ebx
 * regs[2]: ecx
 * regs[3]: edx
 * regs[4]: edi
 * regs[5]: esi
 * regs[6]: ebp
 * regs[7]: eax = esp
 */
.global _swtch
.type _swtch, @function
_swtch:
    leal 4(%esp), %eax           
    movl 4(%esp), %esp
    leal 32(%esp), %esp           

    pushl %eax

    pushl %ebp
    pushl %esi
    pushl %edi
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl -4(%eax)

    movl 4(%eax), %esp

    popl %eax
    popl %ebx
    popl %ecx
    popl %edx
    popl %edi
    popl %esi
    popl %ebp
    popl %esp
    pushl %eax

    xorl %eax, %eax
    ret

#.global __start
#.type __start, @function
#__start:
#    leal 68(%esp), %eax
#    leal 64(%esp), %ebx
#    call *%ebx
#    call Thread_exit;

.global _ENDMONITOR
.type _ENDMONITOR, @function
_ENDMONITOR:
