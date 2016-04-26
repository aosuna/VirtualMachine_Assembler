! main for factorial program
loadi  0 1     ! line 0, R0 = fact(R1)
read   1       ! input R1
call   6       ! call fact
load   0 33    ! receive result of fact
write  0
halt
! fact function
compri 1 1     ! line 6
jumpe  14      ! jump over the recursive call to fact if
jumpl  14      ! R1 is less than or equal 1
call   16      ! call mult (R0 = R0 * R1)
load   0 34    ! receive result of mult
subi   1 1     ! decrement multiplier (R1) and multiply again
call   6       ! call fact
load   0 33
store  0 33    ! line 14, return R0 (result of fact)
return
! mult function
loadi  2 8     ! line 16, init R2 (counter)
oadi  3 0     ! init R3 (result of mult)
shr    1       ! line 18 (loop), shift right multiplier set CARRY
store  2 35    ! save counter
getstat 2      ! to find CARRY's value        
andi   2 1
compri 2 1
jumpe  25      ! if CARRY==1 add
jump   26      ! otherwise do nothing
add    3 0
shl    0       ! make multiplicand ready for next add
load   2 35    ! restore counter
subi   2 1     ! decrement counter        
compri 2 0     ! if counter > 0 jump to loop
jumpg  18
store  3 34    ! return R3 (result of mult)
return
noop           ! line 33, fact return value
noop           ! line 34, mult return value
noop           ! line 35, mult counter