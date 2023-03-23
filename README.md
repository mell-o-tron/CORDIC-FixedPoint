# CORDIC-FixedPoint

A performance test for trigonometric functions, computed with the CORDIC method.

## CORDIC Method

Starting from $v_0 = (1,0)$, a "rotational binary search" is performed, that aims to align $v_i$ with a vector at an angle $\theta$
 from the $x$ axis. Ideally, the rotation step size is initially 45° in either the clockwise or counterclockwise direction, and is then halved at each step until convergence. Practically, since we don't necessarily want to keep a table of values of the tan function, the approximation $\tan \Big( \dfrac{2^{-i} \pi}{4}  \Big) \sim 2^{-i}$ is used. 
 
 Every iteration is computed via a matrix multiplication. Let $\sigma_i$ = i-th step-size:
  
![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/Formulas/for%201.png)

By applying the matrix multiplication repeatedly, we get the following: 

![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/Formulas/for%202.png)

Where $x$ and $y$ are respectively an approximation of the cosine and sine of the angle.

The product of cosines is computed, given the approximation mentioned earlier, as the infinite product $\lim\limits_{n \to \infty}\prod\limits_{i = 0}^{n-1} \dfrac{1}{\sqrt (1 + 2^{-2i})} \sim 0.607252935$ 

## Functions

The functions implemented here are different implementations of the cosine function. The data refers to the performance on my own, rater crappy, 2015 laptop computer (`Intel(R) Core(TM) i5-4210U CPU @ 1.70GHz`)

### Floating point implementations

The first version uses an implementation of the atan function I found on stackoverflow to compensate for the tangent approximation. For 5000000 iterations:

```
execution time in seconds : 7.060013
CO-2eq: 0.014708
```

![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/float.png)


The second version does not compensate for the approximation at all. For 5000000 iterations:

```
execution time in seconds : 1.049544
CO-2eq: 0.002187
```

![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/float_approx.png)


The third version uses a lookup table of `atan` values. For 5000000 iterations:

```
execution time in seconds : 1.757478
CO-2eq: 0.003661
```


![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/float_lookup.png)


### Fixed point implementations

The first fixed point version does not compensate for the approximation. For 5000000 iterations:

```
execution time in seconds: 0.399688
CO-2eq: 0.000833
```

![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/fixed_approx.png)

The second one uses a lookup table of fixed point `atan` values.

```
execution time in seconds: 0.616319
CO-2eq: 0.001284

```

![](https://github.com/mell-o-tron/CORDIC-FixedPoint/blob/main/fixed_lookup.png)


For reference, the standard `math.h` cosine function is executed 5000000 times in just `0.016892` seconds on my setup, lol.
