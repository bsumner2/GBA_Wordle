    .thumb_func
    .section .text
    .align 2
    .global vsync
    .type vsync %function

vsync:
    SVC 0x05
    BX lr
    .size vsync, .-vsync


    .arm
    .section .ewram, "ax", %progbits
    .align 2
    .global isr_handler_cb
    .type isr_handler_cb %function

isr_handler_cb:
    MOV r0, #0x80
    MOV r0, r0, LSL #19
    LDR r1, [r0, #0x200]!
    AND r1, r1, LSR #16

    STRH r1, [r0, #2]
    LDR r2, [r0, #-0x208]!
    ORR r2, r2, r1
    STR r2, [r0]
    
    BX lr
    .size isr_handler_cb, .-isr_handler_cb


    .thumb_func
    .section .text
    .align 2
    .global SRAM_Write
    .type SRAM_Write %function

SRAM_Write:
    // r0 = src addr
    // r1 = # of bytes to write
    // r2 = byte offset
    MOV r3, #0xE0
    LSL r3, r3, #20  // r3 = 0xE0<<20 = 0xE000000 = SRAM start addr
    
    PUSH { r4 }
    ADD r4, r2, r3  // r4 = SRAM_BUF + ofs = write dest start addr = dst
    MOV r2, #0xFF
    LSL r2, r2, #8  // r2 = 0xFF00
    ADD r2, r2, #0xFF  // r2 = 0xFFFF
    ORR r3, r3, r2  // r3 = 0x0E00FFFF = last index of SRAM Buffer
    MOV r2, r4  // r2 = dst
    AND r2, r3, r2  // r2 = dst&0x0E00FFFF

    CMP r2, r4 
    BNE .LSRAM_Write_bad_exit  // if dst&0x0E00FFFF != dst then dst beyond SRAM limit

    ADD r2, r2, r1  // r2 = dst + len

    AND r3, r3, r2  // r3 = (dst+len)&0x0E00FFFF

    CMP r2, r3
    BNE .LSRAM_Write_bad_exit  // if (dst+len)&0x0E00FFFF != (dst+len) then write op will exceed SRAM limit
    
    // Recap:
    // r0= src, r1 = len, r2 = dst+len, r3 = dst+len, r4 = dst
    
    MOV r2, r4
    
    // relevant regs now: r0=src, r1=len, r2=dst, r3 = store/load reg, r4=idx
    MOV r4, #0
    CMP r1, r4
    BEQ .LSRAM_Write_bad_exit

.LSRAM_Write_writeloop:
    
        LDRB r3, [r0, r4]
        STRB r3, [r2, r4]

        ADD r4, r4, #1
    
        CMP r4, r1
        BLT .LSRAM_Write_writeloop

    POP { r4 }
    MOV r0, #1
    BX lr

.LSRAM_Write_bad_exit:
    POP { r4 }
    MOV r0, #0
    BX lr
    .size SRAM_Write, .-SRAM_Write

    .thumb_func
    .section .text
    .align 2
    .global SRAM_Read
    .type SRAM_Read %function
SRAM_Read:

    // r0=dst r1=byte_ct=len r2=ofs
    MOV r3, #0xE0
    LSL r3, r3, #20  // r3 = 0xE0<<20 = 0x0E000000 = SRAM_BUF
    ORR r2, r2, r3  // r2 = ofs|0x0E000000 = [[ only if offset is valid: ]] (ofs+0x0E000000) = SRAM_BUF+ofs
    PUSH { r4 }
    MOV r4, #0xFF
    LSL r4, r4, #8
    ADD r4, r4, #0xFF  // r4 = 0xFF00 + 0xFF = 0xFFFF
    ORR r3, r3, r4  // r3 = 0x0E000000 | 0xFFFF = 0x0E00FFFF
    MOV r4, r2  // r4 = r2 = SRAM_BUF + ofs
    AND r4, r3, r4  // r4 = 0x0E00FFFF & (0x0E000000 + ofs)
    CMP r2, r4  // if (0x0E00FFFF&(SRAM_BUF+ofs)) != (SRAM_BUF+ofs) ...
    BNE .LSRAM_Read_bad_exit  // ... then: Read out of bounds. abort

    ADD r4, r4, r1  // r4 += len := SRAM_BUF+ofs+len
    AND r3, r3, r4  // r3 = 0x0E00FFFF&(SRAM_BUF+ofs+len)
    CMP r3, r4  // if (0x0E00FFFF&(SRAM_BUF+ofs+len)) != (SRAM_BUF+ofs+len)
    BNE .LSRAM_Read_bad_exit  // ... then: Read out of bounds. abort

    // Recap: r0= dst r1=len r2=SRAM_BUF+ofs:=src ; 
    //        r3 and r4 values no longer relevant:
    //        Now r3 :=> idx r4 :=>load/store target reg

    MOV r3, #0
    CMP r3, r1
    BEQ .LSRAM_Read_bad_exit  // If len == 0 then exit. no reading needed

.LSRAM_Read_readloop:
        LDRB r4, [r2, r3]
        STRB r4, [r0, r3]
        ADD r3, r3, #1
        CMP r3, r1
        BLT .LSRAM_Read_readloop

    POP { r4 }
    MOV r0, #1
    BX lr

.LSRAM_Read_bad_exit:
    POP { r4 }
    MOV r0, #0
    BX lr

    .size SRAM_Read, .-SRAM_Read

    .arm
    .section .ewram, "ax", %progbits
    .align 2
    .global fast_memcpy32
    .type fast_memcpy32 %function
fast_memcpy32:
    // Regs
    // r0 Dest ptr
    // r1 Src ptr
    // r2 word count
    AND r12, r2, #7  // r12 will have remainder words
    MOVS r2, r2, LSR #3  // r2 will have word block count
    BEQ .Lfast_memcpy32_remainder_loop
    PUSH {r4-r10}
.Lfast_memcpy32_main_loop:
        LDMIA r1!, { r3-r10 }
        STMIA r0!, { r3-r10 }
        SUBS r2, #1
        BNE .Lfast_memcpy32_main_loop
    POP {r4-r10}
.Lfast_memcpy32_remainder_loop:
        SUBS r12, #1
        LDRCS r3, [r1], #4
        STRCS r3, [r0], #4
        BCS .Lfast_memcpy32_remainder_loop
    BX lr
    .size fast_memcpy32, .-fast_memcpy32

    .arm
    .section .ewram, "ax", %progbits
    .align 2
    .global fast_memset32
    .type fast_memset32 %function
fast_memset32:
    // Regs
    // r0 Dest
    // r1 Fill Value
    // r2 Word count

    AND r12, r2, #7    // r12 = 0b111&word_ct = word_ct%8 = remainder words that can't be block filled.
    MOVS r2, r2, LSR #3    // r2 = r2>>3 = word_ct>>3 = word_ct/8 = word_block_ct
    BEQ .Lfast_memset32_remainder_loop

    PUSH {r4-r9}

    MOV r3, r1
    MOV r4, r1
    MOV r5, r1
    MOV r6, r1
    MOV r7, r1
    MOV r8, r1
    MOV r9, r1
.fast_memset32_main_loop:
        STMIA r0!, {r1, r3-r9}
        SUBS r2, #1
        BNE .fast_memset32_main_loop
    POP {r4-r9}
.Lfast_memset32_remainder_loop:
        SUBS r12, #1
        STRCS r1, [r0], #4
        BCS .Lfast_memset32_remainder_loop
    BX lr
    .size fast_memset32, .-fast_memset32

    .thumb_func
    .section .text
    .align 2
    .global mode3_draw_rect
    .type mode3_draw_rect %function
    
mode3_draw_rect:
    // r0: struct rect
    PUSH { r4-r7, lr }
    LDM r0, {r0, r1, r4, r5, r6}
    
    SUB r2, r4, #1
    CMP r2, #0
    BLT .Lm3_drw_rct_exit
    ADD r2, r2, r0
    CMP r2, #240
    BGE .Lm3_drw_rct_exit
    
    SUB r2, r5, #1
    CMP r2, #0
    BLT .Lm3_drw_rct_exit
    ADD r2, r2, r1
    CMP r2, #160
    BGE .Lm3_drw_rct_exit
    
    LSL r2, r1, #4  // r2 = y*16
    SUB r2, r2, r1  // r2 = (y*16) - y = y*15
    LSL r2, r2, #4  // r2 = y*15*16=y*240
    ADD r0, r0, r2  // r0= x+(y*SCRN_WIDTH)

    MOV r2, #0xC0
    LSL r2, r2, #19  // r2 = VRAM addr
    LSL r0, r0, #1  // r0 = 2*(x+(y*SCR_W))
                    // = (2 bytes/pixel * rect top-left corner pixel offset ) rect top-left corner VRAM byte offset

    ADD r7, r0, r2  // r7 = VRAM addr for start of rect
    LSL r1, r6, #16  // r1 = color<<16
    ORR r6, r6, r1  // r6 = (color<<16) | color
    MOV r0, #3
    AND r0, r0, r7
    CMP r0, #0
    BEQ .Lm3_drw_rct_word_align_loop_first_pass
    
.Lm3_drw_rct_no_align_loop:
        MOV r0, r7
        MOV r1, r6
        STRH r1, [r0]
        ADD r0, r0, #2
        SUB r2, r4, #1
        LSR r2, r2, #1
        LDR r3, =fast_memset32
        BL .Lm3_drw_rct_call_via_r3
        MOV r2, #1
        AND r2, r2, r4
        CMP r2, #1
        BEQ .Lm3_drw_rct_no_align_loop_no_remainder
        SUB r2, r4, #1
        LSL r2, r2, #1
        STRH r6, [r7, r2]
.Lm3_drw_rct_no_align_loop_no_remainder:
        SUB r5, r5, #1
        CMP r5, #0
        BEQ .Lm3_drw_rct_exit
        MOV r0, #240
        LSL r0, r0, #1
        ADD r7, r7, r0
        B .Lm3_drw_rct_no_align_loop


.Lm3_drw_rct_word_align_loop:
        MOV r0, #240
        LSL r0, r0, #1
        ADD r7, r7, r0
.Lm3_drw_rct_word_align_loop_first_pass:
        MOV r0, r7
        MOV r1, r6
        LSR r2, r4, #1
        LDR r3, =fast_memset32
        BL .Lm3_drw_rct_call_via_r3
        MOV r2, #1
        AND r2, r2, r4
        CMP r2, #1
        BNE .Lm3_drw_rct_wrd_al_loop_no_remainder
        
        SUB r2, r4, #1
        LSL r2, r2, #1
        STRH r6, [r7, r2]
.Lm3_drw_rct_wrd_al_loop_no_remainder:
        SUB r5, r5, #1
        CMP r5, #0
        BNE .Lm3_drw_rct_word_align_loop
        

.Lm3_drw_rct_exit:
    POP { r4-r7 }
    POP { r3 }
.Lm3_drw_rct_call_via_r3:
    BX r3
    .size mode3_draw_rect, .-mode3_draw_rect

    
