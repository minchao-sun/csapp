# Part I: Code Injection Attacks
# Level 1
cat ctarget.l1.txt | ./hex2raw | ./ctarget -q
# Level 2
cat ctarget.l2.txt | ./hex2raw | ./ctarget -q
# Level 3
cat ctarget.l3.txt | ./hex2raw | ./ctarget -q

# Part II: Return-Oriented Programming
# level 2
cat rtarget.l2.txt | ./hex2raw | ./rtarget -q
