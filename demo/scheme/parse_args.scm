#! /usr/local/bin/guile -s
!#
(define cmm (command-line))
(display "Program name: ")
(display (car cmm))  
(newline)
(define args (cdr cmm))
(define long (length args))
(define loop (lambda (count len obj)
	(if (<= count len)
		(begin
			(display "argument ")
			(display count)
			(display " : ")
			(display (list-ref obj (- count 1)))
			(newline)
			(set! count (+ count 1))
			(loop count len obj)))))
(loop 1 long args)
