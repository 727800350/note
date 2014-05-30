#!/usr/bin/guile -s
!#
(define space "*")
;; print a list
(define print-list
  (lambda (para)
	(if (null? (cdr para))
	  ;; only one element left
	  (begin (display (car para)) (newline))
	  ;; more than one elements
	  (begin (display (car para)) (display space) (print-list (cdr para))))))
(print-list (list 1 2 3))

;; print out variable number arguments 
(define print
  (lambda (arg . args)
	(display arg)
	(display space)
	(print-list (list args))
;; 	(for-each (lambda (x) (display x) (display space)) args)
;; 	(newline)
  )
)
;; demo of print
(print "this is" (list 1 2 3) "what")
(print "this is a test" 1 2)

;; test if it is zero
(define iszero?
  (lambda (x)
	(if (= x 0) #t #f)))
(print (iszero? 0))
