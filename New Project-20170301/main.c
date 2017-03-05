#include <stdio.h>

typedef enum { 
    NONE, 
    INTERRUPT, 
    SHARED, 
    MODIFY, 
    EXCLUSIVE 
} state;

typedef enum {
    NO_OP,
    CPU_READ,
    CPU_WRITE
} operation;

typedef enum {
    NO_MEM_OP,
    HIT,
    HITM
} mem_op;

// func declarations
state CC_ProcessorAccessingMemory(state my_state, operation my_operation, mem_op my_memop1, mem_op my_memop2);

int main()
{
    printf("TestCase1: Interrupt/Cpu Read/(HIT || HITM) : Expected Output - Memory Read in SHARED state\n");
    printf("=============================================================================================\n");
    if(CC_ProcessorAccessingMemory(INTERRUPT, CPU_READ, HIT, HITM) == SHARED)
    {
        printf("Shared state Returned\n");
    }
    printf("=============================================================================================\n\n\n");
    
    printf("TestCase2: Interrupt/Cpu Write/X : Expected Output - RFO in MODIFY state\n");
    printf("=============================================================================================\n");
    if(CC_ProcessorAccessingMemory(INTERRUPT, CPU_WRITE, HIT, HITM) == MODIFY)
    {
        printf("Modify state Returned\n");
    }
    printf("=============================================================================================\n\n\n");
    
    printf("TestCase3: Interrupt/Cpu Read/(HIT || HITM)' : Expected Output - Memory Read in EXCLUSIVE state\n");
    printf("=============================================================================================\n");
    if(CC_ProcessorAccessingMemory(INTERRUPT, CPU_READ, NO_MEM_OP, NO_MEM_OP) == EXCLUSIVE)
    {
        printf("Exclusive state Returned\n");
    }
    printf("=============================================================================================\n\n\n");
    
    printf("TestCase4: Shared/Cpu Read/X' : Expected Output - Return Shared state\n");
    printf("=============================================================================================\n");
    if(CC_ProcessorAccessingMemory(SHARED, CPU_READ, NO_MEM_OP, NO_MEM_OP) == SHARED)
    {
        printf("Shared state returned\n");
    }
    printf("=============================================================================================\n\n\n");
    
    printf("TestCase5: Shared/Cpu Write/X' : Expected Output - Return Modify state\n");
    printf("=============================================================================================\n");
    if(CC_ProcessorAccessingMemory(SHARED, CPU_WRITE, NO_MEM_OP, NO_MEM_OP) == MODIFY)
    {
        printf("Modify state returned\n");
    }
    printf("=============================================================================================\n\n\n");
    
    printf("TestCase6: Modify/Cpu Read/X' : Expected Output - Return Modify state\n");
    printf("=============================================================================================\n");
    if(CC_ProcessorAccessingMemory(MODIFY, CPU_READ, NO_MEM_OP, NO_MEM_OP) == MODIFY)
    {
        printf("Modify state returned\n");
    }
    printf("=============================================================================================\n\n\n");
    
    printf("TestCase7: Modify/Cpu Write/X' : Expected Output - Return Modify state\n");
    printf("=============================================================================================\n");
    if(CC_ProcessorAccessingMemory(MODIFY, CPU_WRITE, NO_MEM_OP, NO_MEM_OP) == MODIFY)
    {
        printf("Modify state returned\n");
    }
    printf("=============================================================================================\n\n\n");
    
    printf("TestCase8: Exclusive/Cpu Read/X' : Expected Output - Return Exclusive state\n");
    printf("=============================================================================================\n");
    if(CC_ProcessorAccessingMemory(EXCLUSIVE, CPU_READ, NO_MEM_OP, NO_MEM_OP) == EXCLUSIVE)
    {
        printf("Exclusive state returned\n");
    }
    printf("=============================================================================================\n\n\n");
    
    printf("TestCase9: Exclusive/Cpu Write/X' : Expected Output - Return Modify state\n");
    printf("=============================================================================================\n");
    if(CC_ProcessorAccessingMemory(EXCLUSIVE, CPU_WRITE, NO_MEM_OP, NO_MEM_OP) == MODIFY)
    {
        printf("Modify state returned\n");
    }
    printf("=============================================================================================\n\n\n");    
    
    return 0;
}

/*
Func        : Cache Controller of Processor Accessing Memory
Description :
Inputs      : State, Operation, HIT, HITM
Output      : Final State
*/
state CC_ProcessorAccessingMemory(state my_state, operation my_operation, mem_op my_memop1, mem_op my_memop2)
{
    switch(my_state)
    {
        // INTERRUPT state
        case INTERRUPT:
                    if(my_operation == CPU_READ)
                    {
                        if(my_memop1 == HIT || my_memop2 == HITM)
                        {
                            printf("INTERRUPT/CPU_READ/(HIT || HITM)/Memory Read\n");
                            // recall w/ SHARED state & CPU_READ operation 
                            return CC_ProcessorAccessingMemory(SHARED, my_operation, NO_MEM_OP, NO_MEM_OP);
                        }
                        if(!(my_memop1 == HIT || my_memop2 == HITM))
                        {
                            printf("INTERRUPT/CPU_READ/(HIT || HITM)'/Memory Read\n");
                            // recall w/ SHARED state & CPU_READ operation 
                            return CC_ProcessorAccessingMemory(EXCLUSIVE, my_operation, NO_MEM_OP, NO_MEM_OP);
                        }
                    }
                    else if(my_operation == CPU_WRITE)
                    {
                        printf("INTERRUPT/CPU_WRITE/X/RFO\n");
                        // recall w/ MODIFY state & ?? operation
                        return CC_ProcessorAccessingMemory(MODIFY, my_operation, NO_MEM_OP, NO_MEM_OP);
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
                        return CC_ProcessorAccessingMemory(MODIFY, my_operation, NO_MEM_OP, NO_MEM_OP);
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
                        // return EXCLUSIVE state
                        printf("EXCLUSIVE/CPU_WRITE/X/X\n");
                        return CC_ProcessorAccessingMemory(MODIFY, my_operation, NO_MEM_OP, NO_MEM_OP);
                    }
                    break;
    }

}
