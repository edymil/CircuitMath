# CircuitMath
Automatic generation of electric circuit equations + solution.

For documentation see:

L Toscano, S Stella and E Milotti, "Using graph theory for automated electric circuit solving", Eur. J. Phys. 36 (2015) 035015

Use the following command to compile the C code (with gcc)

gcc -W circuitg.c algorithm.c verso.c creatematrix.c ludcmp.c createb.c lubksb.c mprove.c sovr.c tension.c nrutil.c -o circuitg.out

The program uses C code from Numerical Recipes (ludcmp.c, nrutil.c, lubksb.c).



If you use this code, please acknowledge the paper and this repo, and if possible notify me. 
