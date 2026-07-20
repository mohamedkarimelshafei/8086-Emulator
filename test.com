mov ah,09h
mov dx,msg
int 21h
mov ax,4c00h
int 21h

msg db 'Hello from emulator!$'
