; kernel.asm
; Beroperasi dalam mode 16-bit (Real Mode) untuk panggilan BIOS

[BITS 16]

section .text

; Mengekspor simbol agar bisa dipanggil dari C
global _putInMemory
global _interrupt_invoke ; Sesuai nama di template Anda
global getBiosTick


getBiosTick: 
    mov ah, 0x00    ; Layanan BIOS: Get System Time / Tick Count
    int 0x1A        ; Panggil interrupt jam BIOS
    mov ax, dx      ; Pindahkan low-word dari tick count (DX) ke AX (return value utama)
    mov dx, cx      ; Pindahkan high-word dari tick count (CX) ke DX (untuk C jika bisa baca DX:AX)
    ret


_putInMemory:
    push bp         ; Simpan base pointer lama
    mov bp, sp      ; Set base pointer baru ke stack pointer saat ini

    push ds         ; Simpan Data Segment register lama

    mov ax, [bp+4]  ; Ambil parameter 'segment'
    mov ds, ax      ; Set DS dengan nilai segment yang diberikan
    mov si, [bp+6]  ; Ambil parameter 'address' (offset) ke SI
    mov cl, [bp+8]  ; Ambil parameter 'character' ke CL (byte)
    
    mov [si], cl    ; Tulis karakter ke memori di DS:[SI]

    pop ds          ; Kembalikan DS lama
    pop bp          ; Kembalikan base pointer lama
    ret


_interrupt_invoke:
    push bp         ; Simpan base pointer lama
    mov bp, sp      ; Set base pointer baru

  
    mov ax, [bp+4]  ; Ambil parameter 'int_num' ke AX. Kita hanya butuh AL (byte rendah)
                    ; untuk memodifikasi byte nomor interrupt.
    
    push ds         ; Simpan DS lama
    mov bx, cs      ; Asumsikan 'intr_template' berada di Code Segment (.text)
    mov ds, bx      ; Set DS = CS agar bisa menulis ke 'intr_template'
    mov si, intr_template ; Arahkan SI ke template instruksi interrupt
    
 
    mov [si+1], al  
    pop ds          ; Kembalikan DS lama

    ; --- Isi register dengan parameter yang diberikan dari C ---
    mov ax, [bp+6]  ; Isi AX dengan ax_param
    mov bx, [bp+8]  ; Isi BX dengan bx_param
    mov cx, [bp+10] ; Isi CX dengan cx_param
    mov dx, [bp+12] ; Isi DX dengan dx_param

intr_template:
    db 0xCD         ; Opcode untuk instruksi INT (misal, INT 0x10 -> 0xCD 0x10)
    db 0x00         ; Placeholder untuk nomor interrupt (akan diisi oleh kode di atas)


    pop bp          
    ret            

	
