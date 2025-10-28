s_s segment stack "stack"
	dw 32 dup(?)
s_s ends

d_s segment
	nach db 4 dup (0)
	editor db 4 dup (0)
	boof db 5,0,5 dup('$')
	message1 db 'Enter a number: $'
	message2 db 'Answer1: $'
	message3 db 'Answer2: $'
	message4 db ' - even number $'
	message5 db ' - odd number $'
d_s ends

c_s segment
	assume ds:d_s, cs:c_s, ss:s_s
begin:
	mov ax, d_s
	mov ds, ax
;Vvod
	mov ah, 09h
	mov dx, offset message1
	int 21h
	
	call input
	call changeline
;Vivod
	mov ah, 09h
	mov dx, offset message2
	int 21h
	call filleditor
	call reverse
	call output
	

	mov ah, 09h
	mov dx, offset message3
	int 21h
	call filleditor
	call smena
	call output

	mov ah, 4ch
	int 21h
;Procedures
	chetnost proc near
		xor ax, ax
		xor bx, bx
		mov cx, 4
		mov si, 0

		cikl:
			mov al, editor[si]
			test al, 1
			jz cht
			jmp exit2
			cht: inc bx
			jmp exit2
			
		exit2: inc si
		loop cikl
		
		test bx, 1
		jz cht2
		mov ah, 09h
		mov dx, offset message5
		int 21h
		jmp exit3
		cht2:		
		mov ah, 09h
		mov dx, offset message4
		int 21h
		jmp exit3
	exit3:
	ret
	chetnost endp

	reverse proc near
		xor ax, ax
		mov si, 0
		mov al, editor[si]
		mov si, 3
		mov ah, editor[si]
		mov editor[si], al
		mov si, 0
		mov editor[si], ah

		mov si, 1
		mov al, editor[si]
		mov si, 2
		mov ah, editor[si]
		mov editor[si], al
		mov si, 1
		mov editor[si], ah
	ret
	reverse endp

	smena proc near
		xor ax, ax
		mov si, 0
		mov al, editor[si]
		mov si, 1
		mov ah, editor[si]
		mov editor[si], al
		mov si, 0
		mov editor[si], ah

		mov si, 2
		mov al, editor[si]
		mov si, 3
		mov ah, editor[si]
		mov editor[si], al
		mov si, 2
		mov editor[si], ah
	ret
	smena endp

	changeline proc near  
   		mov ah, 0eh       
   		mov al, 0ah   ; next stroka    
   		int 10h        

   		mov ah, 0eh       
   		mov al, 0dh   ; v nachalo    
   		int 10h        
   	ret
	changeline endp

	fillmas proc near
		xor ax, ax
		xor bx, bx
		mov bl, 4
		mov al, cl
		sub bl, al

		metmet1:
			mov ah, boof[si+2]
			sub ah, 30h
			mov nach[si+bx], ah
			inc si
		loop metmet1
	ret
	fillmas endp

	input proc near
		xor ax, ax
		mov ah, 0ah
		lea dx, boof
		int 21h

		mov cl, [boof+1]
		mov si, 0
		call fillmas
	ret
	input endp
	
	output proc near
		xor ax, ax
		xor cx, cx
		mov si, 0
		mov cl, [boof+1] 

		metmet:
			mov dl, editor[si]

			cmp dl, 0
			je null
			
			jmp exit

			null: sub cx, 1
			inc si
			jmp metmet
		
		exit:
		metmet2:
			mov ah, 02h
			mov dl, editor[si]
			add dl, 30h
			int 21h		
			inc si
		loop metmet2

		call chetnost
	
		call changeline
	ret
	output endp

	
	filleditor proc near
		mov cx, 4
		mov si, 0
		copy:
			mov ah, nach[si]
			mov editor[si], ah
			inc si
		loop copy
	ret
	filleditor endp
c_s ends
end begin