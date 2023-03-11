# CORDIC-FixedPoint

A performance test for trigonometric functions, computed with the CORDIC method.

## CORDIC Method

Starting from $v_0 = (1,0)$, a "rotational binary search" is performed, that aims to align $v_i$ with a vector at an angle $\theta$
 from the $x$ axis. Ideally, the rotation step size is initially 45° in either the clockwise or counterclockwise direction, until convergence. Practically, since we don't necessarily want to keep a table of values of the tan function, the approximation $\tan \Bigg( \dfrac{\pi}{4} 2^{-i} \Bigg) \sim 2^{-i}$ is used. 
 
 Every iteration is computed via a matrix multiplication. Let $\sigma_i$ = i-th step-size:
  
![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/Formulas/for%201.png)

By applying the matrix multiplication repeatedly, we get the following: 

![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/Formulas/for%202.png)

Where $x$ and $y$ are respectively an approximation of the cosine and sine of the angle.

The product of cosines is computed, given the approximation mentioned earlier, as the infinite product $\lim\limits_{n \to \infty}\prod\limits_{i = 0}^{n-1} \dfrac{1}{\sqrt (1 + 2^{-2i})} \sim 0.607252935$ 

## Functions

The three functions implemented here are different implementations of the cosine function. Two of them are floating point versions, one of them compensating for the tangent approximation:


![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/float_not_approx.png)


Another that does not compensate:


![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/float_approx.png)

And a fixed point implementation of the former:


![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/fixed.png)

## Performance

As obvious, the approximate floating point function computes faster than the compensating one, and the fixed point implementation beats both of the floating point ones. 
