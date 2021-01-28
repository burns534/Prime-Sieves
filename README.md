# Prime-Sieves

Attempting to convert a prime sieve concept I wrote in Python in 2018 to C++. The idea was to use a wheel sieve to generate candidates on a rolling basis and recursively use primes found from previous calls to increase the wheel size. This only worked for a certain range of primes. I can think of more robust ways to do this now using a bitmap or the sieve of Atkin, but it was part of the learning process.
