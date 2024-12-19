# Advent Of Code

Thank you to [Eric Wastl](https://was.tl/) for creating [Advent of Code](https://adventofcode.com/).

### Method

My method of solving the Advent of Code problems is to read the problem description,
think of a straight-forward way to represent the problem input as a data structure,
which is influenced by my initial thoughts about how to code an algorithm to obtain the required
answer. Then write the code to first read the input file into the data structure and
then implement the algorithm to get the answer. I try this on the worked example given
in the problem description, then on the real puzzle input. I hesitate for a short
while, looking at the code to see if I've made some silly mistake that was not revealed
by the code working correctly for the given worked example. Finally, I submit my answer and hope to get a gold star like I'm
still in primary school (a fairly consistent feeling for me). Once I have the correct
answer I move on to solving part two in a similar way. Sometimes the data structure
I chose doesn't work so well for part two, so it has to be changed and part one modified.

Once I have the correct answer to both parts I look at the code again to see if there
are some simple optimisations that will improve the performance. Often there are.
Sometimes something strikes me and I realise there is a much simpler way to solve it.
(I don't always have the interest or patience to spend much time on optimisation.)

I remember reading one AoC puzzle that said something like "it's 25 milliseconds to
midnight on December 24 so just enough time to solve all 25 puzzles." So I have it in my
head that any solution that takes more than a millisecond is doing it the wrong way.
Often my solutions are nowhere near 1 millisecond. Eventually I've had enough and
publish it to GitHub. Sometimes at this point I look at
[Will Killian's](https://github.com/willkill07/AdventOfCode2024) solution to the
problem: it will invariably be very different to mine and much faster. This is very good
for me because it means I learn something new. It also makes it a little less fun
because I don't look forward to seeing how far off the mark I might be. So sometimes I
don't look at anyone else's solutions until AoC is done and dusted for the year.
