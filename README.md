# memory-consistency
C++ Memory Consistency Simulation
Memory Consistency Model Simulator
# Objective
 As taught during the class, memory consistency models play a major rule on the performance of modern processor systems. To better understand how memory consistency models work, this project is aimed to teach you how different memory consistency models affect performance.
 
 In this project, you will be provided with input files that represent a trace of instructions, similar to the following but much longer traces:

    Load A
    Store A
    Lock1
    Load B
    Store C
    Store D
    Store F
    Load E
    Unlock1
    Store C
    Load D
…
    
# Assumptions
 Assume that you have an infinite cache size with hit latency of 10 cycles. Meanwhile, any access that isn’t a hit in the cache will incur 100 cycles. Also, assume that lock consists of a load and store, i.e., atomic read-modify-write. Meanwhile, unlock is a store instruction. However, lock and unlock are correctly identified by the compiler as acquire and release, respectively, and hence exposed to the hardware as such, if needed and supported. Lock and unlock takes ten cycles if the lock variable is a cache hit, and 100 cycles otherwise. The hardware can issue one instruction at any time, but can complete several ones in the same cycle, i.e., several instructions can have the same finish time, but only one instruction can start at any cycle. For data dependency, for the sake of simplicity, we assume any load/store operations using the same variable are dependent and hence need to be ordered strictly.

# Additional Examples
 For all the cases of PC, SC, WO and RC- above we said that the hardware can issue one instruction at any time, but can complete several ones in the same cycle, i.e., several instructions can have the same finish time, but only one instruction can start at any cycle- implement this as “in any cycle only 1 instruction can be fetched but it can be placed in an issue queue of infinite size till it can get issued and you can issue multiple instructions in the same cycle and multiple instructions can complete/retire in the same cycle.”
 For instance in Weak Ordering: (this makes it slightly easier to reason with and implement it) 

 Instruction
 Hit/Miss
 {Fetch_cycle, Issue_cycle, Retire_cycle}

 Store Q
 miss
 { 0, 0, 100}
 Store S
 miss
 { 1, 1, 101}
 Load I
 miss
 { 2, 2, 102}
 Lock2
 miss
 {3, 102, 202}

 Store G
 miss
 { 4, 202, 302}
 Store G
 hit
 { 5, 302, 312}
 Load D
 miss
 { 6, 202, 302}
 Unlock2
 hit
 { 7, 312, 322}

 Load X
 miss
 { 8, 322, 422}
 Lock15
 miss
 { 9, 422, 522}
 Store W
 miss
 { 10, 522, 622}
 Store K
 miss
 { 11, 522, 622}
 Unlock15
 hit
 { 12, 622, 632}
 Store U
 miss
 { 13, 632, 732}

# PC model
 For processor consistency (PC) model, assume a store/write buffer with size of 16 entries. Only when an entry is evicted, it becomes globally visible. In the PC model, we assume that stores take one cycle to be inserted to the store buffer, and evictions from the store buffer happens only when it is full. For simplicity, when the buffer is full, inserting an entry to the front will evict the last entry. Assume the eviction happens in the background and we only incur the one cycle latency to insert the entry. 
## Specific assumptions for PC (as posted in piazza)
 1. For any store, the retire-time is 1 clock cycle into the store buffer from the time it is issued or executed.
 a. check if the store buffer is full; 
 if (SB_full) {
 evict last_entry_SB; //eviction from SB will complete in 100 cycles if this address has never been brought to the cache before; otherwise complete in 10 cycles.
 //this can happen at the background and does not have to be accounted for while calculating latency but keep track of what is evicted from the SB and when that retires into the cache as it will matter for following unlock 
 } else {
 SB.push(current_store);
 }
 b. if this is a store for unlock 
 flush all the entries of store_buffer. 
 //these can happen in parallel; so if all the stores are a hit in cache, after 10 cycles, retire all these stores to the cache; the store for unlock can finish again taking 10 cycles; so the total completion time for an unlock is 20 cycles from the time of issue.
 //if there is even 1 store that was a miss in the cache, count 100 cycles for the store to retire to the cache after it is brought from memory and then 10 cycles for the unlock to retire. So, total 110 cycles for the unlock to complete from the time of issue. 
 
 if there were previously evicted stores from the SB that have not yet completed i.e not yet written to cache- account for them too; essentially an unlock instruction will act as a fence for all the stores previously present.  
 
 2. If the store is followed by a load to the same address and it has not been evicted from the store buffer to the cache yet, then serve this load as a miss for the first time around. This load could be from another processor and hence, it will be a load miss in the worst case as the store is still sitting in the Store buffer of a different core.   
 Any load/store to the same address after that can be a hit.
 
 Instruction
 Hit/Miss
 {Fetch_cycle, Issue_cycle, Retire_cycle}
 Comments/issue and retire from Store Buffer
 Store Q
 miss
 { 0, 0, 1}
 { 0, 302, 402}
 Store S
 miss
 { 1, 1, 2}
 { 1, 302, 402}
 Load I
 miss
 { 2, 2, 102}
 
 
 Lock2
 miss
 { 3, 102, 202}
 
 
 Store G
 miss
 { 4, 202, 203}
 
 
 Store G
 hit
 { 5, 203, 204}
 { 5, 302, 402}
 Load D
 miss
 { 6, 202, 302}
 
 
 Unlock2
 hit
 { 7, 402, 412}
 Unlock can issue at 302 after the Load D completes; so flush all the stores in the store buffer at 302; they complete at 402; so the unlock issues at 402 and completes at 412 to the cache completely.
 …
 …
 
 Store M
 miss
 { 23, 937, 938}
 { 23, 1149, 1249}
 Store A
 miss
 { 24, 938, 939}
 { 24, 1149, 1159}
 Lock5
 miss
 { 25, 939, 1039}
 
 
 Load A
 hit
 { 26, 1039, 1139}
 Even though this should be a hit, as the store prior is not yet written to the cache, serve this load as a miss.
 
 
 Expected Output: The output from your simulator should be the following:
 Total number of cycles to finish executing the code
 Average number of cycles spent in the critical sections counting for each from the start of executing lock until the lock release has completed and propagated to other cores.
 
 Example Output:
 235 22
 
 The first number (235) is the number of cycles to finish executing all instructions in the application. The second number is the total number of cycles inside the critical sections averaged over the total number of critical sections. Note that for the second number, approximate it to the closest integer value.


# Deliverables
 - Your code along with a Makefile to compile it and make sure it works on grendel.
   The contents should be all *cpp, *h files, Makefile that creates a final executable named sim_program2
 - Do not include the test files given to you and any object files; we will compile your code and create a new executable anyway.
 - 'sim_program2' should accept two arguments for being able to run.
   ./sim_program2 <mem_consistency_model> <input_trace_file>
   where mem_consistency_model is SC, PC, WO or RC keywords and input_trace_file is one of the test files provided.
 - A report labelled- report.pdf
 - These things should all be present inside a folder labelled “program2” (including the report) and submit a compressed version of this using tar czf program2.tgz program2
# Grade Breakdown
 - Reasonable coding efforts 30%
 - Accuracy of the output 60% (there will be 3 test files provided and 2 mysterious test not provided, each for 10%)
 - Report 20%

# Report Structure
 Compare and discuss the execution time for each of the traces between the following consistency models: 
 SC
 WO
 RC
 PC [Optional for ECE406 students]
 Compare the average critical section time for each of the traces between the following consistency models: 
 SC
 WO
 RC
 PC [Optional for ECE406 students]
 
