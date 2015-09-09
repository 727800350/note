":"; exec guile $0 "$@"
(define cmm (command-line))
(display (string-append "Program name: " (car cmm)))
(newline)

(define loop 
	(lambda (count len obj)
		(if (<= count len)
			(begin
				(display "argument ")
				(display count)
				(display " : ")
				(display (list-ref obj (- count 1)))
				(newline)
				(set! count (+ count 1))
				(loop count len obj)
			)
		)
	)
)

(define args (cdr cmm))
(define long (length args))
(loop 1 long args)

