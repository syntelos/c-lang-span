ASCII group spans

  The ASCII code defines four asymmetric group character pairs:
    < >
    { }
    [ ]
    ( ) .

  These characters establish data-code groups as syntactic
  spans that typically include nesting.  Examples include
  LISP and SGML.

Design

  This package holds responsibility for parsing mechanical
  data-code groups formed by asymmmetric character pairs.

References

  [LISP] Recursive functions of symbolic expressions and their computation by machine,
         John McCarthy.  Communications of the ACM, 1960.

  [SGML] The Roots of SGML, Charles F. Goldfarb.
         http://www.sgmlsource.com/history/roots.htm
