;; The eventual return value needs to be a string for proper
;; type support.   Otherwise, leave the casting to the GDScript side.
(define (all-strings s)
  (if (string? s) s (number->string s)))

;; Uncomment this to run a REPL for Geiser to connect to
(use-modules (system repl server))
(spawn-server (make-tcp-server-socket #:port 37146))

(define (doubler n)
  (+ n n))

;; Mutates state outside of scheme in base cpp class
(my-add (doubler 32))
(my-add (doubler 2))

;; Lets us call to a GDScript function we register
(my-fn "sline" 0 0 0 0 0)
(my-fn "line" 0 0 0 0 0)

;; Draw a rectangle, neat!
(define (draw-rect)
  (my-fn "line" 0 0 300 0 0)
  (my-fn "line" 300 0 300 300 0)
  (my-fn "line" 300 300 0 300 0)
  (my-fn "line" 0 300 0 0 0))

(draw-rect)

(all-strings
 (doubler 5))
