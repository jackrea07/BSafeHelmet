; G8RTOS_SchedulerASM.s
; Created: 2022-07-26
; Updated: 2022-07-26
; Contains assembly functions for scheduler.

	; Functions Defined
	.def G8RTOS_Start, PendSV_Handler

	; Dependencies
	.ref CurrentlyRunningThread, G8RTOS_Scheduler

	.thumb		; Set to thumb mode
	.align 2	; Align by 2 bytes (thumb mode uses allignment by 2 or 4)
	.text		; Text section

; Need to have the address defined in file
; (label needs to be close enough to asm code to be reached with PC relative addressing)
RunningPtr: .field CurrentlyRunningThread, 32

; G8RTOS_Start
;	Sets the first thread to be the currently running thread
;	Starts the currently running thread by setting Link Register to tcb's Program Counter
G8RTOS_Start:

	.asmfunc

	;Load the address of RunningPtr
	ldr r3, RunningPtr
	;Load the currently running pointer
	ldr r2, [r3]
	ldr r2, [r2]
	;Load the first thread's stack pointer
	mov sp, r2
	;Load LR with the first thread's PC
	add r2, r2, #56
	ldr lr, [r2]

	BX LR				;Branches to the first thread

	.endasmfunc

; PendSV_Handler
; - Performs a context switch in G8RTOS
; 	- Saves remaining registers into thread stack
;	- Saves current stack pointer to tcb
;	- Calls G8RTOS_Scheduler to get new tcb
;	- Set stack pointer to new stack pointer from new tcb
;	- Pops registers from thread stack
PendSV_Handler:

	.asmfunc

	CPSID I
	PUSH {R4 - R11}

	ldr r3, RunningPtr
	ldr r2, [r3]

	str sp, [r2]
	mov r4, lr

	bl G8RTOS_Scheduler

	mov lr, r4

	ldr r1, [r3]
	ldr sp, [r1]

	POP {R4 - R11}
	CPSIE I

	BX LR

	.endasmfunc

	; end of the asm file
	.align
	.end
