;; The eventual return value needs to be a string for proper
;; type support.   Otherwise, leave the casting to the GDScript side.
(define (all-strings s)
  (if (string? s) s (number->string s)))

;; Uncomment this to run a REPL for Geiser to connect to
(use-modules (system repl server))
(spawn-server (make-tcp-server-socket #:port 12345))

(define (doubler n)
  (+ n n))

;; Mutates state outside of scheme
(my-add (doubler 32))
(my-add (doubler 2))

(all-strings
 (doubler 5))
