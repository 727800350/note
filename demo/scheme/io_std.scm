":"; exec guile $0 "$@"
;; read from stdin and write to stdout

(use-modules (ice-9 readline))
;; (activate-readline)

;; read a line
(display (readline))
(define s (readline))

;; read a s-expression
(display (read))
(newline)

;; read a char
(display (read-char))
(newline)

