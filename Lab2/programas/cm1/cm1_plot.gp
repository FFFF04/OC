#!/usr/bin/gnuplot

set terminal
show terminal
set logscale x 2;
plot "cm1_plot.dat" using 1:2 with lines title "8kB",  \
     "cm1_plot.dat" using 1:3 with lines title "16kB", \
     "cm1_plot.dat" using 1:4 with lines title "32kB", \
     "cm1_plot.dat" using 1:5 with lines title "64kB";

pause mouse



cache_size=8192 stride=1        avg_misses=0.000247     avg_time=0.002853
cache_size=8192 stride=2        avg_misses=0.000147     avg_time=0.003089
cache_size=8192 stride=4        avg_misses=0.000068     avg_time=0.003063
cache_size=8192 stride=8        avg_misses=0.000065     avg_time=0.002978
cache_size=8192 stride=16       avg_misses=0.000058     avg_time=0.003033
cache_size=8192 stride=32       avg_misses=0.000063     avg_time=0.003004
cache_size=8192 stride=64       avg_misses=0.000051     avg_time=0.002638
cache_size=8192 stride=128      avg_misses=0.000024     avg_time=0.002247
cache_size=8192 stride=256      avg_misses=0.000015     avg_time=0.002079
cache_size=8192 stride=512      avg_misses=0.000010     avg_time=0.001996
cache_size=8192 stride=1024     avg_misses=0.000009     avg_time=0.001949
cache_size=8192 stride=2048     avg_misses=0.000009     avg_time=0.002023
cache_size=8192 stride=4096     avg_misses=0.000009     avg_time=0.002078

cache_size=16384        stride=1        avg_misses=0.000230     avg_time=0.002226
cache_size=16384        stride=2        avg_misses=0.000217     avg_time=0.002205
cache_size=16384        stride=4        avg_misses=0.000182     avg_time=0.002192
cache_size=16384        stride=8        avg_misses=0.000178     avg_time=0.002132
cache_size=16384        stride=16       avg_misses=0.000181     avg_time=0.002183
cache_size=16384        stride=32       avg_misses=0.000190     avg_time=0.002191
cache_size=16384        stride=64       avg_misses=0.000168     avg_time=0.002135
cache_size=16384        stride=128      avg_misses=0.000079     avg_time=0.002128
cache_size=16384        stride=256      avg_misses=0.000028     avg_time=0.002028
cache_size=16384        stride=512      avg_misses=0.000025     avg_time=0.001960
cache_size=16384        stride=1024     avg_misses=0.000008     avg_time=0.001936
cache_size=16384        stride=2048     avg_misses=0.000005     avg_time=0.002041
cache_size=16384        stride=4096     avg_misses=0.000005     avg_time=0.002139
cache_size=16384        stride=8192     avg_misses=0.000004     avg_time=0.002025

cache_size=32768        stride=1        avg_misses=0.002177     avg_time=0.002166
cache_size=32768        stride=2        avg_misses=0.003135     avg_time=0.002159
cache_size=32768        stride=4        avg_misses=0.005031     avg_time=0.002124
cache_size=32768        stride=8        avg_misses=0.008600     avg_time=0.002079
cache_size=32768        stride=16       avg_misses=0.015974     avg_time=0.001986
cache_size=32768        stride=32       avg_misses=0.025518     avg_time=0.001953
cache_size=32768        stride=64       avg_misses=0.027554     avg_time=0.002199
cache_size=32768        stride=128      avg_misses=0.051749     avg_time=0.002168
cache_size=32768        stride=256      avg_misses=0.000325     avg_time=0.002137
cache_size=32768        stride=512      avg_misses=0.000152     avg_time=0.002019
cache_size=32768        stride=1024     avg_misses=0.000067     avg_time=0.001953
cache_size=32768        stride=2048     avg_misses=0.000030     avg_time=0.002008
cache_size=32768        stride=4096     avg_misses=0.000021     avg_time=0.002144
cache_size=32768        stride=8192     avg_misses=0.000004     avg_time=0.002128
cache_size=32768        stride=16384    avg_misses=0.000003     avg_time=0.002036

cache_size=65536        stride=1        avg_misses=0.015657     avg_time=0.001958
cache_size=65536        stride=2        avg_misses=0.031310     avg_time=0.001865
cache_size=65536        stride=4        avg_misses=0.062675     avg_time=0.002109
cache_size=65536        stride=8        avg_misses=0.125305     avg_time=0.002178
cache_size=65536        stride=16       avg_misses=0.250541     avg_time=0.002244
cache_size=65536        stride=32       avg_misses=0.501105     avg_time=0.001988
cache_size=65536        stride=64       avg_misses=1.000821     avg_time=0.001851
cache_size=65536        stride=128      avg_misses=1.027360     avg_time=0.001836
cache_size=65536        stride=256      avg_misses=1.037313     avg_time=0.001858
cache_size=65536        stride=512      avg_misses=1.048458     avg_time=0.001888
cache_size=65536        stride=1024     avg_misses=1.010165     avg_time=0.001886
cache_size=65536        stride=2048     avg_misses=1.567662     avg_time=0.004166
cache_size=65536        stride=4096     avg_misses=1.166939     avg_time=0.007372
cache_size=65536        stride=8192     avg_misses=0.000015     avg_time=0.002188
cache_size=65536        stride=16384    avg_misses=0.000001     avg_time=0.002172
cache_size=65536        stride=32768    avg_misses=0.000001     avg_time=0.002078