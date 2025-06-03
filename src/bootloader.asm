
bits 16
ORG 0x7C00

KERNEL_LOAD_SEGMENT equ 0x1000 
KERNEL_LOAD_OFFSET  equ 0x0000 
KERNEL_SECTORS    equ 36     
KERNEL_START_SECTOR equ 2     


%macro PrintChar 1
    mov ah, 0x0E    
    mov al, %1      
    mov bh, 0x00    
    mov bl, 0x0F    
    int 0x10
%endmacro


start:
 
    cli                     ; Nonaktifkan interrupt
    mov ax, 0x07C0          ; Beberapa BIOS mengatur CS ke 0x07C0, IP ke 0x0000
                            ; Atau CS=0x0000, IP=0x7C00. Kita set SS ke segmen yang sama
                            ; atau ke 0x0000 jika CS adalah 0x0000
    mov ds, ax              ; Set DS = ES = SS = 0x07C0 (atau 0x0000 jika ax dari CS adalah 0x0000)
    mov es, ax
    mov ss, ax
    mov sp, 0x7000          ; Stack pointer aman di bawah 0x7C00 jika SS=0x0000
                            ; Jika SS=0x07C0, maka SP bisa 0x0000 (relatif ke 0x7C00, tumbuh ke bawah)
                            ; Untuk konsistensi dengan tes "XY" yang berhasil, jika Anda menggunakan
                            ; SS=0x0000 dan SP=0x7000, pertahankan itu.
                            ; Jika CS saat ORG 0x7C00 adalah 0x0000, maka SS=0x0000, SP=0x7000 aman.
    sti                     ; Aktifkan kembali interrupt

    PrintChar '1'           

    mov ax, KERNEL_LOAD_SEGMENT
    mov es, ax              
    mov bx, KERNEL_LOAD_OFFSET 
                           

  
    mov ah, 0x02            
    mov al, KERNEL_SECTORS  
    mov ch, 0x00            
    mov cl, KERNEL_START_SECTOR 
    mov dh, 0x00            
    mov dl, 0x00            

    PrintChar '2'           

  
    int 0x13

    PrintChar '3'           


    jc .disk_read_error     

    PrintChar '4'           

 
    mov ax, KERNEL_LOAD_SEGMENT
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0xFFF0          
    mov bp, sp

    PrintChar '5'           

    
    jmp KERNEL_LOAD_SEGMENT:KERNEL_LOAD_OFFSET


.disk_read_error:
    PrintChar 'E'           
    mov si, error_message
.print_error_loop:
    lodsb                   
    or al, al               
    jz .hang                
    PrintChar al            
    jmp .print_error_loop

.hang:
    PrintChar 'H'          
    cli
    hlt

error_message:
    db ' Bootloader: Disk read error! ', 0x0D, 0x0A, 0


    times 510 - ($-$$) db 0
    dw 0xAA55
