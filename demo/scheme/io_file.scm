":"; exec guile $0 "$@"
;; read from a file and write to a file

(define input (open-input-file "input"))
(define output (open-output-file "output"))

;; (use-modules (ice-9 readline))
;; (activate-readline)
;; (display (readline input) output)

(display (read input) output)

(display (read-char input) output)

(close-input-port input)
(close-output-port output)

