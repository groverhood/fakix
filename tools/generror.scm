#!/usr/bin/guile --no-debug
!#

(use-modules (ice-9 textual-ports))
(use-modules (ice-9 format))
(define error-files (cdr (program-arguments)))

(define (default-eval expr)
    (eval expr (interaction-environment)))

(define (println . rest)
    (apply display rest)
    (newline))

(define (error suffix info)
    (list #t suffix info))

(define (success suffix)
    (list #f suffix))
    
(define (error-group prefix . errors)
    (map 
        (lambda (error)
            (let ((error? (car error)) (attributes (cdr error)))
                (if error?
                    (append 
                        (list #t (string-append prefix "_" (car attributes)))
                        (cdr attributes))
                    (list #f (string-append prefix "_" (car attributes))))))
        errors))

(define (define-errors . groups)
    (apply append groups))

(define (eval-input-list filename)
    (default-eval 
        (read (open-input-string 
            (call-with-input-file filename get-string-all)))))

(define errors-list (eval-input-list (car error-files)))

(define (write-error info number)
    (format #t "#define ~a ~d\n" (car info) number)
    (if (= number 0)
        (format #t "#define ~a_ERRSTR ~s\n" (car info) "no errors encountered")
        (format #t "#define ~a_ERRSTR ~s\n" (car info) (car (cdr info)))))


(display "#ifndef FAKIX_ERRORS_H\n#define FAKIX_ERRORS_H 1\n\n")
(let loop ((n 1) (errors errors-list))
    (if (not (null? errors))
        (let ((err (car errors)))
            (if (not (car err))
                (begin 
                    (write-error (cdr err) 0)
                    (loop n (cdr errors)))
                (begin
                    (write-error (cdr err) n)
                    (loop (+ n 1) (cdr errors)))))))
(display "\n#endif\n")