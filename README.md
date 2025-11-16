# julian-date

Julian dates let you do date calculations without messing about with the often complex details of traditional calendars.

Tools are needed to convert between calendar dates and Julian dates.

The problem is that most implementations of these conversions:
- don't support negative Julian dates 
- read like cryptic incantations addressed to a long-forgotten god

This little project provides an algorithm for Julian dates that:
- doesn't restrict the date in any way
- has a large number of unit tests
- is not cryptic, and can be read and understood 

Implementations are provided in C99 and Java.

It's somewhat suprising that so many tools restrict the date: 
- the <a href='https://www.iausofa.org/'>SOFA 2023-10-11</a> library from the IAU (International Astronomical Union)
- the <a href='https://www.astropy.org/'>astropy 7.1</a> project, which uses SOFA
- the <a href='https://aa.usno.navy.mil/software/novas_info'>NOVAS 3.1</a> library from the U.S. Naval Observatory
- the book <a href='img/astro-algo-1991.jpg'>*Astronomical Algorithms*</a> by Jean Meeus, 1991

(Here's an <a href='img/jpl-2025-11-15.jpg'>example</a> taken from an <a href='https://ssd.jpl.nasa.gov/tools/jdc/#/cd'>online calculator</a> 
from the JPL that's just plain wrong.
The input date is the origin of the Julian period as expressed in the Julian calendar. In the Gregorian calendar, 
that day is -4713-11-24.)



## References

The *Explanatory Supplement* (1992), <a href='https://archive.org/details/explanatorysuppl0003unse/page/604/mode/2up'>page 604</a> 
has an implementation that <a href='img/expl-supp-1992.jpg'>looks like this</a>.

The *Explanatory Supplement* (2006), <a href='https://archive.org/details/explanatorysuppl00pken/page/604/mode/2up'>page 604</a> repeats the same algorithm, 
shown <a href='img/expl-supp-2006.jpg'>here</a>.

The above two references are considered authoritative by professional astronomers. 
Both of these references cite <a href='https://dl.acm.org/doi/pdf/10.1145/364096.364097'>this 1968 paper</a> 
by Fliegel and van Flandern, published in the *Communications of the ACM*.
An image of the core logic is <a href='img/comm-acm-1968.jpg'>here</a>.


## Modern Astronomy

A desire for an ultra-compact implementation is understandable for 1968. 
But is it still appropriate in the modern world?
Fundamental astronomy has made giant strides in the past decades:
- measurements of stellar positions, proper motions, and radial velocities from instruments such as GAIA are remarkably precise
- modern models of the Earth's precession and nutation provide accurate results stretching back tens of thousands of years

Perhaps it's time to update the Julian date algorithm, and drop the restriction on dates.








