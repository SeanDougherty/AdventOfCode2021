# Advent of Code 2021

### Day 1

Straightforward. Mistakenly wrote over part1 with part2. I enjoyed the small optimization made when comparing 'average' depth for part2. Instead of recalculating three number sums in each iteration, I opted to copy the current depth to 'prev' and update the new depth via curr = prev - input[i-3] + input[i];

### Day 2

Straightforward. Not much to say here!

### Day 3

Really enjoyed this one! Building a trie was a fun solution and the idea came to me pretty quickly! Definitely had to refactor it once or twice, but we made it there in the end and the final Trie class and main() look really clean.

### Day 4

A good exercise in OOP, I did well, but there were definitely some improvements that could be had.

I got some great feedback from someone on ways to improve this solution:

> You have some needless copies e.g. copying boards[draw] to toBeMarked;

> When passing in read-only references to functions always use const otherwise you don't know if the function will mutate it

> Since you seem to be making an attempt to make it efficient with the unordered_map stuff, why not store the index of the number in the map too? you can have unordered_map<int, vector<tuple<Board\*, size_t, size_t>>>

> You seem to be using hardcoded sentinel values like -5, best to put those in a > constexpr constant at the top of some file. also your code will fail if there are negative numbers in the input

> Also you have trivial getters which are not good: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-get

> Better to make a const member if you want something to be read-only

> Your getVals function has for (int j=0; j < this->board[0].size(); j++) { instead of board[i].size() but nowhere do you seem to check that each row is the same length

### Day 5

Really sloppy with this one. Not proud of it either. The plus side is that I took a moment to finally learn a bit about regex! ...even if the regex I ended up writing was extremely basic.

### Day 6

This was a fun one. Used direct simulation for part 1. Part 2 completely tore that apart thanks to exponential explosion. The fix came within a few seconds of thought, which I'm pretty happy about. Anyways, not a bad day overall.

### Day 7

This one was brute-forced. I never like settling for the brute-force solution but the optimal solution for this one relied on a bit of math knowledge that I wasn't familiar with. Glad to know that it wasn't me just missing an algorithmic approach or forgetting data structure. Also, I wasn't sure what the equation was for the "Sum" version of a factorial. A quick wiki search told me it was the Triangular Sum formula. I knew an equation had to exist, I just hadn't done much math work in some time. In any case, it was an alright challenge and I got to test my automated AoC util service! A few bugs on it's first run, but I'm expecting much better results when I test it again tonight for Day 8!

### Day 8

This was a challenging one, both logically and with the use of proper data structures. I eventually found a rather elegant approach to part2 that I am happy with. However, one critique would be that there is no handling of malformed input and I could do better to implement better exception throwing/handling. Alltogether still an enjoyable challenge. And as a bonus, my utility worked well!

### Day 9

Nothing to report, solved very quickly with a WFS (whatever-first-search).

### Day 10

A little awkward writing out the mappings, otherwise this was a pretty simple stack solution.

### Day 11

Really wanted to solve in O(mn) but had to settle for O(2\*mn). Part 2 was a pretty quick solve thanks to my approach on tracking flashes per step.

### Day 12

This was a fun alteration to traditional graph traversal. My solution was pretty verbose, and I think I could've done better to encapsulate certain aspects of the code and to avoid encapsulating other aspects. Checking for presence in the set was clunky and my choices for "cantAdd" or "!isLarge" was pretty atrocious, "canAdd" and "isSmall" would've been much better methods. Also, a "isInPath()" method would have been very nice.

### Day 13

My mental approach was near solid, it was just a matter of doing better to check edge cases. I lucked out and passed part 1 even with a bug in my code. The bug was a missing '% line' at the end of the updated x or y coord. This was a really fun challenge. I liked the index manipulation that is involved in 'folding' matrices.

### Day 14

This was a pretty straightforward sol'n. I had considered the non-brute force solution during part 1, but I really just wanted to get part1 submitted so I created the quick and dirty approach for the first half of today's challenge. Ended up paying for this in the end, a few minutes of attempted refactor turned into almost an entire rewrite. And the rewrite was not pretty. But I solved in just over 1hr, not great but I'll take it this late in AoC! Spent a bit of time this morning cleaning up part2 for better readability.

### Day 15

I don't think I will ever forget the optimization of dijkstra going forward. The solution was obvious within seconds of reading the problem. I had dijkstra written up from what I could remember within 30 minutes. It solved part 1 and I thought to myself "Ahh I did it! I recalled dijkstra from like 3 years ago in Algo class". Hours later I'm staring at my screen trying to find any little optimization or any silly time waste I'm doing in the code. Part 2 expanded the graph 25x over and now my beautiful dijkstra method can't even return a value. In a moment of weakness I looked at a colleagues solution that had already solved it. The difference? A little optimization that is taught to everyone right after the simple dijkstra approach is taught. (Simple dijkstra is O(V^2)). If you store 'open' vertices in a priority queue instead of looping over the entire graph each time to find the 'next smallest unvisited node' your runtime speeds up from O(V^2) to O(V + ElogV). I begrudgingly make the change and watch as my code completes in less than a second. I look at the clock, it's nearly 3AM and my only thought is: "Had I remembered the priority queue speed up, I could've been in bed 3 hours ago".

Let's just hope a night like that doesn't happen again.
Altogether a good challenge and a good lesson.

### Day 16

A lot of people are saying they really liked this challenge. To me it was just alright. Seemed like a pretty straight-forward translation of requirements. No fun mental challenge. No real trick to solving it. It was pretty easy, just time consuming. There weren't many places to get performance boosts either from what I could find. Oh well. Part 1 and Part 2 are both contained within `part1.cpp`

### Day 17

Definitely overthought this one. I think the late night attempts are starting to get to me. No more staying up until midnight for this guy. Hopefully tomorrow I think more clearly.

### Day 18

This one was really fun to solve. I am a fan of tree data structures, and decoding the tree representation of the string input was pretty fun. Additionally, coming up with the traversals for both explode and split were enjoyable. I've seen some solutions that do some sort of "flattening". Not too sure how that functions, but I'd be interested to learn the approach.

### Day 19

My god, that tested me. Within a couple minutes I had identified that you could find pairings of scanners by comparing the distance between siblings for each beacon in a scanner. If BeaconA is from scanner1 and BeaconB is from scanner2, and BeaconA has the same 11 distances from its siblings as BeaconB has from its siblings. Then they must be the same beacon, and their matching distances must be to the remaining shared 11 beacons.

After that I tried to implement rotation using wikipedia's (Rotation Matrix)[https://en.m.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions]. As I knew you would have to line up the shared points by rotating them. However, I was confused because you could rotate each of the 3 axis' into 4 different states (0, 90, 180, 270), which would be 4^3 different orientations (...or so I thought). It wasn't clear to me how there could be only 24 orientations until I got this whole rotation matrix stuff out of my head.

By the time I had done that, and found my own rotation lookup table, my code had been put into a rather big mess. This wasn't suitable so I ended up rewriting the entire codebase from scratch.

This time, I took my time and was deliberate with the commenting, architecting, and programming. It resulted in much fewer errors and a much smoother development process. I need to take this understanding with me forward onto future problems.

Had I taken it slower in my design and approach, I might've solved this problem before 24hours. Although, the rotations did do a real number on me. So maybe I wouldn't have anyway.

### Day 20

Pretty tricky today, having to find a solution for the flashing border was time consuming. Won't pretend this code is written well. If I hadn't also done Day 19 today,
I would likely have spent more energy on writing Day 20 a bit more cleanly.

### Day 21

This feels like a slight reprieve. However, I can tell that the difficulty overall has ramped up. Day 19 still feels the most challenging, but that's not to say Day 20 and 21 were a cake walk. As for today, I had a hard time coming up with an approach for part 2. Finally, after a hint, I learned it was a 2D DP problem. It took me about 15-20min to settle on the final structure for the DP table and then the implementation was actually really enjoyable to produce.

### Day 22

I love the end result for this one, it was a fun challenge overall to think about.
I even designed it with Part 2 in mind, knowing that tracking 3d space directly would've
been a fools errand. When I finally had a program working for the Part 1 example I was ready to fire off the code for my given input and join the "Sub-1minute part2" club.
I speedily entered the total cubes lit (including the initialization area) and was
befuddled when it came back as incorrect. A quick test later and I realized my folly.
Integer overflow ruins me once again. Part 2 was submitted just under 4 minutes later.
Oh well, maybe next time.

P.S. I know 'decompose()' could be simplified down to eliminate code duplication. But it's more trouble than it's worth at this point.
