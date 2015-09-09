":"; exec guile $0 "$@"

;; mutual recursion
(define is-even?
	(lambda (n)
		(if (= n 0) 
			#t 
			(is-odd? (- n 1))
		)
	)
)

(define is-odd?
	(lambda (n)
		(if (= n 0) 
			#f 
			(is-even? (- n 1))
		)
	)
)

(display (is-even? 2))
(newline)

(display (is-odd? 2))
(newline)

