#include <stdio.h>

// These need to move into a new header file
// All bus operations
#define READ         1
#define WRITE        2
#define INVALIDATE   3   
#define RFO          4
#define NO_BUS_OP    0

typedef enum { 
    NONE, 
    INVALID, 
    SHARED, 
    MODIFY, 
    EXCLUSIVE 
} state;

typedef enum {
    NO_OP,
    CPU_READ,
    CPU_WRITE
} cpu_operation;

typedef enum {
    NO_MEM_OP,
    HIT,
    HITM,
    HITB,
    HITMB
} cache_flags;

typedef enum {
    MEM_READ,
    MEM_INVALIDATE,
    MEM_RFO,
    MEM_X
} cpu_out;



// func declarations
int print_cpu_out_op(cpu_out cpu_out_op);
state SD_ProcessorAccessingMemory(state my_state, cpu_operation my_operation, cache_flags my_memop1, cache_flags my_memop2, cpu_out* cpu_op);
state SD_BusAccessingMemory(state my_state, cache_flags my_memop, int bus_operation);


int main()
{
    cpu_out cpu_out_op = MEM_X;
    printf("TestCase1: Interrupt/Cpu Read/(HIT || HITM) : Expected Output - Memory Read in SHARED state\n");
    printf("=============================================================================================\n");
    if(SD_ProcessorAccessingMemory(INVALID, CPU_READ, HIT, HITM, &cpu_out_op) == SHARED)
    {
        printf("Shared state Returned\n");
        print_cpu_out_op(cpu_out_op);
    }
    printf("=============================================================================================\n\n\n");
    cpu_out_op = MEM_X;
    
    
    printf("TestCase2: Interrupt/Cpu Write/X : Expected Output - RFO in MODIFY state\n");
    printf("=============================================================================================\n");
    if(SD_ProcessorAccessingMemory(INVALID, CPU_WRITE, HIT, HITM, &cpu_out_op) == MODIFY)
    {
        printf("Modify state Returned\n");
        print_cpu_out_op(cpu_out_op);
    }
    printf("=============================================================================================\n\n\n");
    cpu_out_op = MEM_X;
    
    
    printf("TestCase3: Interrupt/Cpu Read/(HIT || HITM)' : Expected Output - Memory Read in EXCLUSIVE state\n");
    printf("=============================================================================================\n");
    if(SD_ProcessorAccessingMemory(INVALID, CPU_READ, NO_MEM_OP, NO_MEM_OP, &cpu_out_op) == EXCLUSIVE)
    {
        printf("Exclusive state Returned\n");
        print_cpu_out_op(cpu_out_op);
    }
    printf("=============================================================================================\n\n\n");
    cpu_out_op = MEM_X;
    
    
    printf("TestCase4: Shared/Cpu Read/X' : Expected Output - Return Shared state\n");
    printf("=============================================================================================\n");
    if(SD_ProcessorAccessingMemory(SHARED, CPU_READ, NO_MEM_OP, NO_MEM_OP, &cpu_out_op) == SHARED)
    {
        printf("Shared state returned\n");
        print_cpu_out_op(cpu_out_op);
    }
    printf("=============================================================================================\n\n\n");
    cpu_out_op = MEM_X;
    
    
    printf("TestCase5: Shared/Cpu Write/X' : Expected Output - Return Modify state\n");
    printf("=============================================================================================\n");
    if(SD_ProcessorAccessingMemory(SHARED, CPU_WRITE, NO_MEM_OP, NO_MEM_OP, &cpu_out_op) == MODIFY)
    {
        printf("Modify state returned\n");
        print_cpu_out_op(cpu_out_op);
    }
    printf("=============================================================================================\n\n\n");
    cpu_out_op = MEM_X;
    
    
    printf("TestCase6: Modify/Cpu Read/X' : Expected Output - Return Modify state\n");
    printf("=============================================================================================\n");
    if(SD_ProcessorAccessingMemory(MODIFY, CPU_READ, NO_MEM_OP, NO_MEM_OP, &cpu_out_op) == MODIFY)
    {
        printf("Modify state returned\n");
        print_cpu_out_op(cpu_out_op);
    }
    printf("=============================================================================================\n\n\n");
    cpu_out_op = MEM_X;
    
    
    printf("TestCase7: Modify/Cpu Write/X' : Expected Output - Return Modify state\n");
    printf("=============================================================================================\n");
    if(SD_ProcessorAccessingMemory(MODIFY, CPU_WRITE, NO_MEM_OP, NO_MEM_OP, &cpu_out_op) == MODIFY)
    {
        printf("Modify state returned\n");
        print_cpu_out_op(cpu_out_op);
    }
    printf("=============================================================================================\n\n\n");
    cpu_out_op = MEM_X;
    
    
    printf("TestCase8: Exclusive/Cpu Read/X' : Expected Output - Return Exclusive state\n");
    printf("=============================================================================================\n");
    if(SD_ProcessorAccessingMemory(EXCLUSIVE, CPU_READ, NO_MEM_OP, NO_MEM_OP, &cpu_out_op) == EXCLUSIVE)
    {
        printf("Exclusive state returned\n");
        print_cpu_out_op(cpu_out_op);
    }
    printf("=============================================================================================\n\n\n");
    cpu_out_op = MEM_X;
    
    
    printf("TestCase9: Exclusive/Cpu Write/X' : Expected Output - Return Modify state\n");
    printf("=============================================================================================\n");
    if(SD_ProcessorAccessingMemory(EXCLUSIVE, CPU_WRITE, NO_MEM_OP, NO_MEM_OP, &cpu_out_op) == MODIFY)
    {
        printf("Modify state returned\n");
        print_cpu_out_op(cpu_out_op);
    }
    printf("=============================================================================================\n\n\n");    
    cpu_out_op = MEM_X;
    
    return 0;
}


/*
       FOR TESTING ONLY
Func: Print bus operation status
input: bus_op
output: print bus operation as string
*/
int print_cpu_out_op(cpu_out cpu_out_op)
{
    switch(cpu_out_op)
    {
        case MEM_READ:
                   printf("CPU Operation: MEMORY READ\n");
                   break;
        case MEM_INVALIDATE:
                   printf("CPU Operation: MEMORY INVALIDATE\n");
                   break;
        case MEM_RFO:
                   printf("CPU Operation: MEMORY RFO\n");
                   break;            
    }
    return 0;
}


/*
Func        : Cache Controller of Processor Accessing Memory
Description :
Inputs      : State, Operation, HIT, HITM
Output      : Final State
    MEM_READ,
    MEM_INVALIDATE,
    MEM_RFO,
    MEM_X
*/
state SD_ProcessorAccessingMemory(state my_state, cpu_operation my_operation, cache_flags my_memop1, cache_flags my_memop2, cpu_out* cpu_op)
{
    switch(my_state)
    {
        // INVALID state
        case INVALID:
                    if(my_operation == CPU_READ)
                    {
                        if(my_memop1 == HIT || my_memop2 == HITM)
                        {
                            printf("INVALID/CPU_READ/(HIT || HITM)/Memory Read\n");
                            // recall w/ SHARED state & CPU_READ operation 
                            *cpu_op = MEM_READ;
                            return SD_ProcessorAccessingMemory(SHARED, my_operation, NO_MEM_OP, NO_MEM_OP, cpu_op);
                        }
                        if(!(my_memop1 == HIT || my_memop2 == HITM))
                        {
                            printf("INVALID/CPU_READ/(HIT || HITM)'/Memory Read\n");
                            // recall w/ SHARED state & CPU_READ operation 
                            *cpu_op = MEM_READ;
                            return SD_ProcessorAccessingMemory(EXCLUSIVE, my_operation, NO_MEM_OP, NO_MEM_OP, cpu_op);
                        }
                    }
                    else if(my_operation == CPU_WRITE)
                    {
                        printf("INTERRUPT/CPU_WRITE/X/RFO\n");
                        // recall w/ MODIFY state & ?? operation
                        *cpu_op = MEM_RFO;
                        return SD_ProcessorAccessingMemory(MODIFY, my_operation, NO_MEM_OP, NO_MEM_OP, cpu_op);
                    }
                    break;
        // SHARED state
        case SHARED:
                    if(my_operation == CPU_READ)
                    {
                        // print & return the state
                        printf("SHARED/CPU_READ/X/X\n");
                        return SHARED;
                    }
                    else if(my_operation == CPU_WRITE)
                    {
                        // print & recall w/ MODIFY state & CPU Write operation
                        printf("MODIFY/CPU_WRITE/X/Invalidate*\n");
                        *cpu_op = MEM_INVALIDATE;
                        return SD_ProcessorAccessingMemory(MODIFY, my_operation, NO_MEM_OP, NO_MEM_OP, cpu_op);
                    }
                    break;
        // MODIFY state
        case MODIFY:
                    if(my_operation == CPU_READ || my_operation == CPU_WRITE)
                    {
                        // return MODIFY state
                        printf("MODIFY/CPU_READ||CPU_WRITE/X/X\n");
                        return MODIFY;
                    }
                    break;
        case EXCLUSIVE:
                    if(my_operation == CPU_READ)
                    {
                        // return EXCLUSIVE state
                        printf("EXCLUSIVE/CPU_READ/X/X\n");
                        return EXCLUSIVE;
                    }
                    else if(my_operation == CPU_WRITE)
                    {
                        printf("EXCLUSIVE/CPU_WRITE/X/X\n");
                        return SD_ProcessorAccessingMemory(MODIFY, my_operation, NO_MEM_OP, NO_MEM_OP, cpu_op);
                    }
                    break;
    }

}

/*
* Func Name          : SD_BusAccessingMemory
* Description        : 
* Inputs             : 
@param state         - describe states enum
@param cache_flags   - describe cache flags
@param bus_operation - describe bus operations
Output               : final state
*/
state SD_BusAccessingMemory(state my_state, cache_flags my_memop, int bus_operation)
{
 switch(my_state)
    {
        // INVALID state
        case INVALID:
                    if(bus_operation == NO_BUS_OP)
                    {
                        if(my_memop == HITB)
                        {
                            printf("INVALID/NO_BUS_OP/(!HIT)\n");
                            // return Invalid state
                            return INVALID;
                        }
                     }                         
                      
                 break;
        // SHARED state
        case SHARED:
                    if(bus_operation == READ)
                    {
                            if(my_memop == HIT)
                            {
                                 printf("SHARED/READ/HIT/X\n");
                                 return SHARED;
                            }
                    }
                    else if(bus_operation == RFO || bus_operation == INVALIDATE)
                    {
                        printf("INVALID/X/HITB/X\n");
                        return SD_BusAccessingMemory(INVALID, HITB,NO_BUS_OP);
                    }
                    break;
        // MODIFY state
        case MODIFY:
                    if(bus_operation == RFO )
                    {
                        if(my_memop == HITM)
                        {
                            printf("INVALID/X/HITB/X\n");
                            return SD_BusAccessingMemory(INVALID, HITB,NO_BUS_OP);
                        }
                    }
                    else if(bus_operation == READ)
                    {
                        if(my_memop == HITM)
                        {
                           printf("SHARED/READ/HIT/X");
                           return SD_BusAccessingMemory(SHARED,HIT,READ);
                        }
                    }     
                    break;
        case EXCLUSIVE:
                    if(bus_operation == READ)
                    {
                        printf("SHARED/READ/HIT/X\n");
                        return SD_BusAccessingMemory(SHARED,HIT,READ);;
                    }
                    else if(bus_operation == RFO)
                    {
                        printf("INVALID/X/HITB/X\n");
                        return SD_BusAccessingMemory(INVALID, HITB,NO_BUS_OP);
                    }
                    break;
    }

}