":"; exec guile $0 "$@"

(define factorial
	(lambda (n)
		(if (= n 0) 
		  	1
			(* n (factorial (- n 1)))
		)
	)
)

(display (factorial 5))
(newline)

