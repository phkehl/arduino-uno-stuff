set grid
set yrange [0:100]
set y2range [-50:50]
set ytics 10
set y2tics 10
plot "< tail -200 range.dat" using 0:1   with lines     title "raw"   lc "#ff0000", \
     "< tail -200 range.dat" using 0:2:3 with errorbars notitle       lc "#0000ff" lw 1, \
     "< tail -200 range.dat" using 0:2   with lines     title "mean"  lc "#0000ff" lw 2, \
     "< tail -200 range.dat" using 0:4   with lines     title "filt"  lc "#00ff00" lw 2, \
     "< tail -200 range.dat" using 0:5   axes x1y2 with lines     title "delta" lc "#aaaaaa" lw 1, \

pause 0.2
reread
