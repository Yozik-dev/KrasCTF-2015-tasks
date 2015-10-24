    section .text
    use16

    org 0x7C00

startup:
    
    ; copy main kernel module into memory and jump there

    mov ax, 0x50    ; base offset where the kernel is gonna start
    mov es, ax      ; it has to be loaded into es:bx
    xor bx, bx
    xor ch, ch      ; number of cylinder
    mov cl, 2       ; number of sector, startup code is loaded into the first sector
    xor dx, dx      ; disk and head numbers are eq to zero
    mov ah, 2       ; 2 - is the number of the read function
    mov al, 17      ; amount of sectors to read

    int 0x13

    jc read_error

    jmp 0x500


read_error:

    dec byte [error_count]
    jnz startup

    hlt
    jmp $


error_count db 5

    times 0x1FE - $ + $$ db 0   ; it eq to the 500 - amount of code bytes os zeroes
    db 0x55, 0xAA               ; magic signature