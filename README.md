# CORDIC-FixedPoint

A performance test for trigonometric functions, computed with the CORDIC method.

## CORDIC Method

Starting from $v_0 = (1,0)$, a "rotational binary search" is performed, that aims to align $v_i$ with a vector at an angle $\theta$
 from the $x$ axis. Ideally, the rotation step size is initially 45° in either the clockwise or counterclockwise direction, until convergence. Practically, since we don't necessarily want to keep a table of values of the tan function, the approximation $\tan \Big( \dfrac{2^{-i} \pi}{4}  \Big) \sim 2^{-i}$ is used. 
 
 Every iteration is computed via a matrix multiplication. Let $\sigma_i$ = i-th step-size:
  
![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/Formulas/for%201.png)

By applying the matrix multiplication repeatedly, we get the following: 

![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/Formulas/for%202.png)

Where $x$ and $y$ are respectively an approximation of the cosine and sine of the angle.

The product of cosines is computed, given the approximation mentioned earlier, as the infinite product $\lim\limits_{n \to \infty}\prod\limits_{i = 0}^{n-1} \dfrac{1}{\sqrt (1 + 2^{-2i})} \sim 0.607252935$ 

## Functions

The functions implemented here are different implementations of the cosine function. 

### Floating point implementations

The first version uses an implementation of the atan function I found on stackoverflow to compensate for the tangent approximation. Running time `4.564983 seconds` for 5000000 iterations.

![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/float.png)


The second version does not compensate for the approximation at all. Running time `0.568714 seconds` for 5000000 iterations.


![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/float_approx.png)


The third version uses a lookup table of `atan` values. Running time `0.943545 seconds` for 5000000 iterations.


![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/float_lookup.png)


### Fixed point implementations

The first fixed point version does not compensate for the approximation. Running time `0.245440` for 5000000 iterations.

![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/fixed_approx.png)

The second one uses a lookup table of fixed point `atan` values.  Running time `0.348289` for 5000000 iterations.

![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/fixed_lookup.png)


For reference, the standard `math.h` cosine function is executed 5000000 times in just `0.008872` seconds on my setup, lol.
