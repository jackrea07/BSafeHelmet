// G8RTOS_Scheduler.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for scheduler functions

#include "../G8RTOS_Scheduler.h"

/************************************Includes***************************************/

#include <stdint.h>
#include <stdbool.h>

#include "../G8RTOS_CriticalSection.h"

#include <inc/hw_memmap.h>
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_nvic.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

/********************************Private Variables**********************************/

// Thread Control Blocks - array to hold information for each thread
static tcb_t threadControlBlocks[MAX_THREADS];

// Thread Stacks - array of arrays for individual stacks of each thread
static uint32_t threadStacks[MAX_THREADS][STACKSIZE];

// Periodic Event Threads - array to hold pertinent information for each thread
static ptcb_t pthreadControlBlocks[MAX_PTHREADS];

// Current Number of Threads currently in the scheduler
static uint32_t NumberOfThreads;

// Current Number of Periodic Threads currently in the scheduler
static uint32_t NumberOfPThreads;

static uint32_t threadCounter = 0;

static int32_t threadIDCount = 0;

/*******************************Private Functions***********************************/

// Occurs every 1 ms.
static void InitSysTick(void)
{
    uint32_t clockSpd = SysCtlClockGet();
    uint32_t period = clockSpd/1000;
    // Set systick period
    SysTickPeriodSet(period);
    // Set systick interrupt handler
    SysTickIntRegister(SysTick_Handler);
    // Set pendsv handler
    IntRegister(FAULT_PENDSV, PendSV_Handler);
    // Enable systick interrupt
    SysTickIntEnable();
    // Enable systick
    SysTickEnable();
}


/********************************Public Variables***********************************/

uint32_t SystemTime;

tcb_t* CurrentlyRunningThread;

uint32_t IBit_State;

/********************************Public Functions***********************************/

// SysTick_Handler
// Increments system time, sets PendSV flag to start scheduler.
// Return: void
void SysTick_Handler() {
    SystemTime++;

    tcb_t *pt = CurrentlyRunningThread->nextTCB;
    for(int i = 0; i < NumberOfThreads; i++){
        if((pt->sleepCount <= SystemTime) && (pt->asleep)){
            pt->asleep = false;
            pt->sleepCount = 0;
        }
        pt = pt->nextTCB;
    }

    ptcb_t *ppt = &pthreadControlBlocks[0];
        for(int i = 0; i < NumberOfPThreads; i++){
            if((SystemTime >= ppt->executeTime)){
                ppt->handler();
                ppt->executeTime = SystemTime + ppt->period;
            }
            ppt = ppt->nextPTCB;
        }

    HWREG(NVIC_INT_CTRL) |=
                NVIC_INT_CTRL_PEND_SV;

    // Traverse the linked-list to find which threads should be awake.
    // Traverse the periodic linked list to run which functions need to be run.
}

// G8RTOS_Init
// Initializes the RTOS by initializing system time.
// Return: void
void G8RTOS_Init() {
    uint32_t newVTORTable = 0x20000000;
    uint32_t* newTable = (uint32_t*)newVTORTable;
    uint32_t* oldTable = (uint32_t*) 0;

    for (int i = 0; i < 155; i++) {
        newTable[i] = oldTable[i];
    }

    HWREG(NVIC_VTABLE) = newVTORTable;

    SystemTime = 0;
    NumberOfThreads = 0;
    NumberOfPThreads = 0;
    // Set currently running thread to the first control block
    CurrentlyRunningThread = &threadControlBlocks[0];
}

// G8RTOS_Launch
// Launches the RTOS.
// Return: error codes, 0 if none
int32_t G8RTOS_Launch() {
    // Initialize system tick
    InitSysTick();
    // Set interrupt priorities
       // Pendsv
    HWREG(NVIC_SYS_PRI3) |=
           NVIC_SYS_PRI3_PENDSV_M;
       // Systick
    HWREG(NVIC_SYS_PRI3) |=
            NVIC_SYS_PRI3_TICK_M;
    // Call G8RTOS_Start()
    G8RTOS_Start();

    return 0;
}

// G8RTOS_Scheduler
// Chooses next thread in the TCB. This time uses priority scheduling.
// Return: void
void G8RTOS_Scheduler() {
    // Using priority, determine the most eligible thread to run that
    // is not blocked or asleep. Set current thread to this thread's TCB.
    tcb_t *pt = CurrentlyRunningThread->nextTCB;
    tcb_t *next = 0;
    uint16_t priorityCheck = 256;
    for(int i = 0; i < NumberOfThreads; i++){
        if((pt->blocked == 0) && (pt->asleep == 0)){
            if(pt->priority < priorityCheck){
                next = pt;
                priorityCheck = pt->priority;
            }
        }
        pt = pt->nextTCB;
    }
    CurrentlyRunningThread = next;
}

// G8RTOS_AddThread
// Adds a thread. This is now in a critical section to support dynamic threads.
// It also now should initalize priority and account for live or dead threads.
// Param void* "threadToAdd": pointer to thread function address
// Param uint8_t "priority": priority from 0, 255.
// Param char* "name": character array containing the thread name.
// Return: sched_ErrCode_t
sched_ErrCode_t G8RTOS_AddThread(void (*threadToAdd)(void), uint8_t priority, char *name) {
    // Your code here
    IBit_State = StartCriticalSection();

    uint8_t index = 0;

    for(int i = 0; i < MAX_THREADS; i++){
        if(!threadControlBlocks[i].isAlive){
            index = i;
            break;
        }
    }

    if(NumberOfThreads == MAX_THREADS){
        EndCriticalSection(IBit_State);
        return THREAD_LIMIT_REACHED;
    }
    else
        if(NumberOfThreads == 0){
            threadStacks[0][STACKSIZE - 1] = THUMBBIT;
            threadStacks[0][STACKSIZE - 2] = threadToAdd;
            for(int i = 3; i < 17; i++){
                threadStacks[0][STACKSIZE - i] = 0;
            }
            threadControlBlocks[0].stackPointer = &threadStacks[0][STACKSIZE - 16];
            threadControlBlocks[0].nextTCB = &threadControlBlocks[0];
            threadControlBlocks[0].previousTCB = &threadControlBlocks[0];
            threadControlBlocks[0].priority = priority;
            threadControlBlocks[0].blocked = 0;
            threadControlBlocks[0].isAlive = 1;
            threadControlBlocks[0].ThreadID = threadIDCount;
            strcpy(threadControlBlocks[0].threadName, name);
        }
        else{
            threadStacks[index][STACKSIZE - 1] = THUMBBIT;
            threadStacks[index][STACKSIZE - 2] = threadToAdd;
            for(int i = 3; i < 17; i++){
                     threadStacks[index][STACKSIZE - i] = 0;
            }
            threadControlBlocks[index].stackPointer = &threadStacks[index][STACKSIZE - 16];
            threadControlBlocks[index].previousTCB = CurrentlyRunningThread;
            threadControlBlocks[index].nextTCB = CurrentlyRunningThread->nextTCB;
            threadControlBlocks[index].nextTCB->previousTCB = &threadControlBlocks[index];
            CurrentlyRunningThread->nextTCB = &threadControlBlocks[index];
            threadControlBlocks[index].priority = priority;
            threadControlBlocks[index].blocked = 0;
            threadControlBlocks[index].isAlive = 1;
            threadControlBlocks[index].ThreadID = threadIDCount;
            strcpy(threadControlBlocks[index].threadName, name);
        }
    NumberOfThreads++;
    EndCriticalSection(IBit_State);
    return NO_ERROR;

    // This should be in a critical section!
}

// G8RTOS_Add_APeriodicEvent


// Param void* "AthreadToAdd": pointer to thread function address
// Param int32_t "IRQn": Interrupt request number that references the vector table. [0..155].
// Return: sched_ErrCode_t
sched_ErrCode_t G8RTOS_Add_APeriodicEvent(void (*AthreadToAdd)(void), uint8_t priority, int32_t IRQn) {
    IBit_State = StartCriticalSection();
    // Disable interrupts
    IntMasterDisable();
    // Check if IRQn is valid
    if(IRQn > 154 || IRQn < 0){
        return -6;
    }
    // Check if priority is valid
    if(priority > 6){
        return -7;
    }
    // Set corresponding index in interrupt vector table to handler.
    IntRegister(IRQn, AthreadToAdd);
    // Set priority.
    IntPrioritySet(IRQn, priority);
    // Enable the interrupt.
    IntEnable(IRQn);
    // End the critical section.
    IntMasterEnable();
    EndCriticalSection(IBit_State);
    return 0;
}

// G8RTOS_Add_PeriodicEvent
// Adds periodic threads to G8RTOS Scheduler
// Function will initialize a periodic event struct to represent event.
// The struct will be added to a linked list of periodic events
// Param void* "PThreadToAdd": void-void function for P thread handler
// Param uint32_t "period": period of P thread to add
// Param uint32_t "execution": When to execute the periodic thread
// Return: sched_ErrCode_t
sched_ErrCode_t G8RTOS_Add_PeriodicEvent(void (*PThreadToAdd)(void), uint32_t period, uint32_t execution) {
    IBit_State = StartCriticalSection();
    // your code
    // Make sure that the number of PThreads is not greater than max PThreads.
    if(NumberOfPThreads >= MAX_PTHREADS){
        return -1;
    }
        // Check if there is no PThread. Initialize and set the first PThread.
    if(NumberOfPThreads == 0){
        pthreadControlBlocks[0].nextPTCB = &pthreadControlBlocks[0];
        pthreadControlBlocks[0].previousPTCB = &pthreadControlBlocks[0];
        pthreadControlBlocks[0].handler = PThreadToAdd;
        pthreadControlBlocks[0].currentTime = SystemTime;
        pthreadControlBlocks[0].period = period;
        pthreadControlBlocks[0].executeTime = SystemTime + execution;
    }
        // Subsequent PThreads should be added, inserted similarly to a doubly-linked linked list
            // last PTCB should point to first, last PTCB should point to last.
    else{
        pthreadControlBlocks[NumberOfPThreads].previousPTCB = &pthreadControlBlocks[NumberOfPThreads - 1];
        pthreadControlBlocks[NumberOfPThreads].nextPTCB = &pthreadControlBlocks[0];
        pthreadControlBlocks[0].previousPTCB = &pthreadControlBlocks[NumberOfPThreads];
        pthreadControlBlocks[NumberOfPThreads - 1].nextPTCB = &pthreadControlBlocks[NumberOfPThreads];
        pthreadControlBlocks[NumberOfPThreads].handler = PThreadToAdd;
        pthreadControlBlocks[NumberOfPThreads].currentTime = SystemTime;
        pthreadControlBlocks[NumberOfPThreads].period = period;
        pthreadControlBlocks[NumberOfPThreads].executeTime = SystemTime + execution;
    }
        // Set function
        // Set period
        // Set execute time
        // Increment number of PThreads
    NumberOfPThreads++;
    EndCriticalSection(IBit_State);

    return NO_ERROR;
}

// G8RTOS_KillThread
// Param uint32_t "threadID": ID of thread to kill
// Return: sched_ErrCode_t
sched_ErrCode_t G8RTOS_KillThread(threadID_t threadID) {
    // Start critical section
    IBit_State = StartCriticalSection();
    // Check if there is only one thread, return if so
    if(NumberOfThreads <= 1)
        return -5;
    // Traverse linked list, find thread to kill
    tcb_t *pt = CurrentlyRunningThread->nextTCB;
    for(int i = 0; i < NumberOfThreads; i++){
        if(pt->ThreadID == threadID){
            pt->isAlive = false;
            pt->previousTCB->nextTCB = pt->nextTCB;
            pt->nextTCB->previousTCB = pt->previousTCB;
            pt->blocked = 0;
            NumberOfThreads--;
            break;
        }
    }
        // Update the next tcb and prev tcb pointers if found
            // mark as not alive, release the semaphore it is blocked on
        // Otherwise, thread does not exist.

    EndCriticalSection(IBit_State);
    return 0;
}

// G8RTOS_KillSelf
// Kills currently running thread.
// Return: sched_ErrCode_t
sched_ErrCode_t G8RTOS_KillSelf() {
    // your code
    IBit_State = StartCriticalSection();
        // Check if there is only one thread, return if so
    if(NumberOfThreads <= 1)
        return -5;
    // Traverse linked list, find thread to kill
    CurrentlyRunningThread->isAlive = false;
    CurrentlyRunningThread->nextTCB->previousTCB = CurrentlyRunningThread->previousTCB;
    CurrentlyRunningThread->previousTCB->nextTCB = CurrentlyRunningThread->nextTCB;
    CurrentlyRunningThread->blocked = 0;
    NumberOfThreads--;
    HWREG(NVIC_INT_CTRL) |=
                NVIC_INT_CTRL_PEND_SV;
    EndCriticalSection(IBit_State);
    return 0;
    // Check if there is only one thread
    // Else, mark this thread as not alive.
}

// sleep
// Puts current thread to sleep
// Param uint32_t "durationMS": how many systicks to sleep for
void sleep(uint32_t durationMS) {
    // Update time to sleep to
    CurrentlyRunningThread->sleepCount = SystemTime + durationMS;
    // Set thread as asleep
    CurrentlyRunningThread->asleep = true;
    HWREG(NVIC_INT_CTRL) |=
                NVIC_INT_CTRL_PEND_SV;
}

// G8RTOS_GetThreadID
// Gets current thread ID.
// Return: threadID_t
threadID_t G8RTOS_GetThreadID(void) {
    return CurrentlyRunningThread->ThreadID;        //Returns the thread ID
}

// G8RTOS_GetNumberOfThreads
// Gets number of threads.
// Return: uint32_t
uint32_t G8RTOS_GetNumberOfThreads(void) {
    return NumberOfThreads;         //Returns the number of threads
}
