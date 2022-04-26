PROCESSOR 18F8722
    
#include <xc.inc>

; configurations
CONFIG OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

; global variable declarations
GLOBAL var1
GLOBAL _t1, _t2, _t3
GLOBAL b_level, c_mode, d_time, is_open, lat_var, is_open_c, b_mod

; allocating memory for variables
PSECT udata_acs
    var1:
	DS     1    ; allocates 1 byte
    _t1:
	DS 1
    _t2:
	DS 1
    _t3:
	DS 1
    b_level:
	DS 1
    c_mode:
	DS 1
    d_time:
	DS 1
    is_open:
	DS 1
    is_open_c:
	DS 1
    lat_var:
	DS 1
    b_mod:
	DS 1
	

PSECT resetVec,class=CODE,reloc=2
resetVec:
    goto    main
    
delay_1sec:
    movlw 0x84      ; copy desired value to W
    movwf _t3       ; copy W into t3
    _loop3:
        movlw 0xAF      ; copy desired value to W
        movwf _t2       ; copy W into t2
        _loop2:
            movlw 0x8F      ; copy desired value to W
            movwf _t1       ; copy W into t1
            _loop1:
                decfsz _t1, 1   ; decrement t1, if 0 skip next 
                goto _loop1     ; else keep counting down
                decfsz _t2, 1   ; decrement t2, if 0 skip next 
                goto _loop2     ; else keep counting down
                decfsz _t3, 1   ; decrement t3, if 0 skip next 
                goto _loop3     ; else keep counting down
                return
		


    
		
initial:
    clrf PORTB
    clrf PORTC
    clrf PORTD
    
    movlw 0b00010000
    movwf TRISA
    
    clrf LATA

    
    movlw 0b00010000
    movwf TRISE
    
    clrf LATE
    
    
    movlw 0b00000000
    movwf TRISB
    
    movwf TRISC
    
    movwf TRISD
    
    movlw 0xF
    movwf LATB
    movlw 0x03
    movwf LATC
    movlw 0xFF
    movwf LATD
    
    return
    
default:
    movlw 0x01
    movwf LATB
    movwf b_level
    movwf b_mod
    movwf is_open
    movwf is_open_c
    movwf LATC
    
    movlw 0x00
    movwf LATD
    
    movlw 0b00000001
    movwf c_mode
    
    
    return
    

ms500_timer_release:
        movlw 0x1F    
	movwf _t3     
	ms500_timer_release3:
	    movlw 0xAF      
	    movwf _t2       
	    ms500_timer_release2:
		movlw 0x8F
		movwf _t1
		ms500_timer_release1:
		    btfss PORTE, 4
		    goto b_delay_05sec_press
		    decfsz _t1, 1   
		    goto ms500_timer_release1    
		    decfsz _t2, 1  
		    goto ms500_timer_release2    
		    decfsz _t3, 1 
		    goto ms500_timer_release3   
		    call ms500_passed
		    goto ms500_timer_release		    

		    

		
b_delay_05sec_press:
    movlw 0x16      ; copy desired value to W
    movwf _t3       ; copy W into t3
    b_delay_05sec_press1:
        movlw 0xAF      ; copy desired value to W
        movwf _t2       ; copy W into t2
        b_delay_05sec_press2:
            movlw 0x8F      ; copy desired value to W
            movwf _t1       ; copy W into t1
            b_delay_05sec_press3:
		btfsc PORTE, 4  ; re4 pressed?
		goto b_delay_05sec_release3_re4			
		btfsc PORTA, 4
		goto b_delay_05sec_release3_ra4
                decfsz _t1, 1   ; decrement t1, if 0 skip next 
                goto b_delay_05sec_press3     ; else keep counting down
                decfsz _t2, 1   ; decrement t2, if 0 skip next 
                goto b_delay_05sec_press2     ; else keep counting down
                decfsz _t3, 1   ; decrement t3, if 0 skip next 
                goto b_delay_05sec_press1     ; else keep counting down
		call ms500_passed
                call b_blink_states
		goto b_delay_05sec_press
		

		
b_delay_05sec_release_re4:
    movlw 0x16      ; copy desired value to W
    movwf _t3       ; copy W into t3
    b_delay_05sec_release1_re4:
        movlw 0xAF      ; copy desired value to W
        movwf _t2       ; copy W into t2
        b_delay_05sec_release2_re4:
            movlw 0x8F      ; copy desired value to W
            movwf _t1       ; copy W into t1
            b_delay_05sec_release3_re4:
		btfss PORTE, 4
		goto b_to_c
                decfsz _t1, 1   ; decrement t1, if 0 skip next 
                goto b_delay_05sec_release1_re4     ; else keep counting down
                decfsz _t2, 1   ; decrement t2, if 0 skip next 
                goto b_delay_05sec_release2_re4     ; else keep counting down
                decfsz _t3, 1   ; decrement t3, if 0 skip next 
                goto b_delay_05sec_release3_re4     ; else keep counting down
		call ms500_passed
                call b_blink_states
		goto b_delay_05sec_release_re4

b_delay_05sec_release_ra4:
    movlw 0x16      ; copy desired value to W
    movwf _t3       ; copy W into t3
    b_delay_05sec_release1_ra4:
        movlw 0xAF      ; copy desired value to W
        movwf _t2       ; copy W into t2
        b_delay_05sec_release2_ra4:
            movlw 0x8F      ; copy desired value to W
            movwf _t1       ; copy W into t1
            b_delay_05sec_release3_ra4:
		btfss PORTA, 4
		goto states_of_b
                decfsz _t1, 1   ; decrement t1, if 0 skip next 
                goto b_delay_05sec_release1_ra4     ; else keep counting down
                decfsz _t2, 1   ; decrement t2, if 0 skip next 
                goto b_delay_05sec_release2_ra4     ; else keep counting down
                decfsz _t3, 1   ; decrement t3, if 0 skip next 
                goto b_delay_05sec_release3_ra4     ; else keep counting down
		call ms500_passed
                call b_blink_states
		goto b_delay_05sec_release_ra4
		
		
		
c_delay_05sec_press:
    movlw 0x16      ; copy desired value to W
    movwf _t3       ; copy W into t3
    c_delay_05sec_press1:
        movlw 0xAF      ; copy desired value to W
        movwf _t2       ; copy W into t2
        c_delay_05sec_press2:
            movlw 0x8F      ; copy desired value to W
            movwf _t1       ; copy W into t1
            c_delay_05sec_press3:
		btfsc PORTE, 4  ; re4 pressed?
		goto c_delay_05sec_release3_re4			
		btfsc PORTA, 4
		goto c_delay_05sec_release3_ra4
                decfsz _t1, 1   ; decrement t1, if 0 skip next 
                goto c_delay_05sec_press3     ; else keep counting down
                decfsz _t2, 1   ; decrement t2, if 0 skip next 
                goto c_delay_05sec_press2     ; else keep counting down
                decfsz _t3, 1   ; decrement t3, if 0 skip next 
                goto c_delay_05sec_press1     ; else keep counting down
		call ms500_passed
		call c_blink_states
		goto c_delay_05sec_press
		   
c_delay_05sec_release_re4:
    movlw 0x16      ; copy desired value to W
    movwf _t3       ; copy W into t3
    c_delay_05sec_release1_re4:
        movlw 0xAF      ; copy desired value to W
        movwf _t2       ; copy W into t2
        c_delay_05sec_release2_re4:
            movlw 0x8F      ; copy desired value to W
            movwf _t1       ; copy W into t1
            c_delay_05sec_release3_re4:
		btfss PORTE, 4  ; re4 pressed?
		goto c_to_d			
                decfsz _t1, 1   ; decrement t1, if 0 skip next 
                goto c_delay_05sec_release3_re4     ; else keep counting down
                decfsz _t2, 1   ; decrement t2, if 0 skip next 
                goto c_delay_05sec_release2_re4     ; else keep counting down
                decfsz _t3, 1   ; decrement t3, if 0 skip next 
                goto c_delay_05sec_release1_re4     ; else keep counting down
		call ms500_passed
		call c_blink_states
		goto c_delay_05sec_release_re4
		
		
c_delay_05sec_release_ra4:
    movlw 0x16      ; copy desired value to W
    movwf _t3       ; copy W into t3
    c_delay_05sec_release1_ra4:
        movlw 0xAF      ; copy desired value to W
        movwf _t2       ; copy W into t2
        c_delay_05sec_release2_ra4:
            movlw 0x8F      ; copy desired value to W
            movwf _t1       ; copy W into t1
            c_delay_05sec_release3_ra4:
		btfss PORTA, 4
		goto states_of_c
                decfsz _t1, 1   ; decrement t1, if 0 skip next 
                goto c_delay_05sec_release3_ra4     ; else keep counting down
                decfsz _t2, 1   ; decrement t2, if 0 skip next 
                goto c_delay_05sec_release2_ra4     ; else keep counting down
                decfsz _t3, 1   ; decrement t3, if 0 skip next 
                goto c_delay_05sec_release1_ra4     ; else keep counting down
		call ms500_passed
		call c_blink_states
		goto c_delay_05sec_release_ra4
		
		

		

		
delay_05sec_busy:
    movlw 0x42      ; copy desired value to W
    movwf _t3       ; copy W into t3
    __loop33:
        movlw 0xAF      ; copy desired value to W
        movwf _t2       ; copy W into t2
        __loop22:
            movlw 0x8F      ; copy desired value to W
            movwf _t1       ; copy W into t1
            __loop12:
                decfsz _t1, 1   ; decrement t1, if 0 skip next 
                goto __loop12     ; else keep counting down
                decfsz _t2, 1   ; decrement t2, if 0 skip next 
                goto __loop22     ; else keep counting down
                decfsz _t3, 1   ; decrement t3, if 0 skip next 
                goto __loop33     ; else keep counting down
		call ms500_passed
                return
		
		

b_to_c:
    movf b_level, 0
    movwf LATB
    goto c_delay_05sec_press3

c_to_d:
    movf c_mode, 0
    movwf LATC
    goto states_of_d
		
		
states_of_b:
    btfsc b_level, 3
    goto b_state_first
    btfsc b_level, 2
    goto b_state_fourth
    btfsc b_level, 1
    goto b_state_third
    btfsc b_level, 0
    goto b_state_second

states_of_c:
    btfsc c_mode, 0
    goto c_state_defence
    goto c_state_attack
    
states_of_d:
    btfsc c_mode, 0
    goto d_state_attack
    goto d_state_defence

    
b_blink_states:
    btfsc b_level, 3
    goto b_fourth_blink
    btfsc b_level, 2
    goto b_third_blink
    btfsc b_level, 1
    goto b_second_blink
    btfsc b_level, 0
    goto b_first_blink
    
c_blink_states:
    btfsc c_mode, 0
    goto c_state_attack_blink
    goto c_state_defence_blink


b_close:
    clrf LATB
    movlw 0b00000000
    movwf is_open
    return
c_close:
    clrf LATC
    movlw 0b00000000
    movwf is_open_c
    return
		
    

b_state_first:
    movlw 0b00000001
    movwf LATB
    movwf b_level
    movlw 0b00000001
    movwf b_mod
    goto b_delay_05sec_press3

b_first_blink:
    btfss is_open, 0
    goto b_first_open
    goto b_close
b_first_open:
    movlw 0b00000001
    movwf LATB
    movlw 0b00000001
    movwf is_open
    return

    
    
    

    

b_state_second:
    movlw 0b00000011
    movwf LATB
    movwf b_level
    movlw 0b00000010
    movwf b_mod
    goto b_delay_05sec_press3


b_second_blink:
    btfss is_open, 0
    goto b_second_open
    goto b_close
b_second_open:
    movlw 0b00000011
    movwf LATB
    movlw 0b00000001
    movwf is_open
    return
    
b_state_third:
    movlw 0b00000111
    movwf LATB
    movwf b_level
    movlw 0b00000100
    movwf b_mod
    goto b_delay_05sec_press3

    
b_third_blink:
    btfss is_open, 0
    goto b_third_open
    goto b_close
b_third_open:
    movlw 0b00000111
    movwf LATB
    movlw 0b00000001
    movwf is_open
    return
    
b_state_fourth:
    movlw 0b00001111
    movwf b_level
    movwf LATB
    movlw 0b00001000
    movwf b_mod
    goto b_delay_05sec_press3

b_fourth_blink:
    btfss is_open, 0
    goto b_fourth_open
    goto b_close
b_fourth_open:
    movlw 0b00001111
    movwf LATB
    movlw 0b00000001
    movwf is_open
    return    


    
c_state_attack:
    movlw 0b00000001
    movwf LATC
    movwf c_mode
    goto c_delay_05sec_press3

c_state_attack_blink:
    btfss is_open_c, 0
    goto c_attack_open
    goto c_close
c_attack_open:
    movlw 0b00000001
    movwf LATC
    movlw 0b00000001
    movwf is_open_c
    return
    
c_state_defence:
    movlw 0b00000010
    movwf LATC
    movwf c_mode
    goto c_delay_05sec_press3

c_state_defence_blink:
    btfss is_open_c, 0
    goto c_defence_open
    goto c_close
c_defence_open:
    movlw 0b00000010
    movwf LATC
    movlw 0b00000001
    movwf is_open_c
    return
    
    
    ; 0b00000001 = 1
    ; 0b00000010 = 2
    ; 0b00000100 = 3
    ; 0b00001000 = 4


    
    
d_state_attack:
    btfsc b_mod, 0
    goto d_state_attack_1
    btfsc b_mod, 1
    goto d_state_attack_2
    btfsc b_mod, 2
    goto d_state_attack_3
    btfsc b_mod, 3
    goto d_state_attack_4
    
    
d_state_attack_1:
    movlw 0b00000001
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000000
    movwf LATD
    goto ret 
d_state_attack_2:
    movlw 0b00000011
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000001
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000000
    movwf LATD
    goto ret
d_state_attack_3:
    movlw 0b00000111
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000011
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000001
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000000
    movwf LATD
    goto ret
    
d_state_attack_4:
    movlw 0b00001111
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000111
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000011
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000001
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000000
    movwf LATD
    goto ret

    
    
d_state_defence:
    btfsc b_mod, 0
    goto d_state_defence_1
    btfsc b_mod, 1
    goto d_state_defence_2
    btfsc b_mod, 2
    goto d_state_defence_3
    btfsc b_mod, 3
    goto d_state_defence_4
    
d_state_defence_1:
    movlw 0b00000011
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000001
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000000
    movwf LATD
    call delay_05sec_busy
    goto ret
d_state_defence_2:
    movlw 0b00001111
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000111
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000011
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000001
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000000
    movwf LATD
    goto ret
d_state_defence_3:
    movlw 0b00111111
    movwf LATD
    call delay_05sec_busy
    movlw 0b00011111
    movwf LATD
    call delay_05sec_busy
    movlw 0b00001111
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000111
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000011
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000001
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000000
    movwf LATD
    goto ret
    
d_state_defence_4:
    movlw 0b11111111
    movwf LATD
    call delay_05sec_busy
    movlw 0b01111111
    movwf LATD
    call delay_05sec_busy
    movlw 0b00111111
    movwf LATD
    call delay_05sec_busy
    movlw 0b00011111
    movwf LATD
    call delay_05sec_busy
    movlw 0b00001111
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000111
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000011
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000001
    movwf LATD
    call delay_05sec_busy
    movlw 0b00000000
    movwf LATD
    goto ret


    
ret:
    call delay_05sec_busy
    call default
    return
    
    

    
    

    

    
    
    
    

; DO NOT DELETE OR MODIFY
; 500ms pass check for test scripts
ms500_passed:
    nop
    return

; DO NOT DELETE OR MODIFY
; 1sec pass check for test scripts
ms1000_passed:
    nop
    return
	
PSECT CODE
main:
    ; some code to initialize and wait 1000ms here, maybe
    call initial
    call delay_1sec
    call ms1000_passed
    call default
    ; a loop here, maybe
    loop:
        ; inside the loop, once it is confirmed 500ms passed
	movlw 0x1F    
	movwf _t3     
	ms500_timer3:
	    movlw 0xAF      
	    movwf _t2       
	    ms500_timer2:
		movlw 0x8F
		movwf _t1
		ms500_timer1:
		    btfsc PORTE,4
		    call ms500_timer_release1  
		    decfsz _t1, 1   
		    goto ms500_timer1    
		    decfsz _t2, 1  
		    goto ms500_timer2    
		    decfsz _t3, 1 
		    goto ms500_timer3   
		    call ms500_passed
		    goto loop
	
	
		    

end resetVec
