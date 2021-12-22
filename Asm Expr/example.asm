mov eax, 1; eax = 1
add eax, 2; eax = 1+2
inc eax; eax = 1+2
sub eax, 3; eax = (1+2+1)-3

mov ebx, 3; eax = 3
mul eax, ebx; eax = 3*((1+2+1)-3)

mov ebx, 1; ebx = 1
add ebx, 4; ebx = 1+4

mov edx, 5; edx = 5
mul ebx, edx; ebx = 5*(1+4)

sub ebx, eax; edx = (5*(1+4) - 3*((1+2+1)-3))
mov eax, 10; eax = 10
sub eax, 9; eax = 10 - 9
div ebx, eax; ebx = (5*(1+4) - 3*((1+2)-3))/(10-9)
dec ebx; (5*(1+4) - 3*((1+2)-3))/(10-9) - 1 

out ebx; ((5*(1+4) - 3*((1+2)-3))/(10-9) - 1)
