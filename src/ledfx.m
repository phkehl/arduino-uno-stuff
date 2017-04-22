clear all;
close all;

## 5mm and 25mm WS2801 LEDs ix0 Ray:
##  polyfit([ 50 100 150 200 250 255 ], [ 1.25 1.75 2 2.25 2.75 3.0 ], 1) # one channel    --> 0.00767824497257779  0.88056063375990334
##  polyfit([ 50 100 150 200 250 255 ], [ 1.75 2.25 3 3.75 4.25 4.5 ], 1) # two channels   --> 0.0133455210237660   1.0146252285191966
##  polyfit([ 50 100 150 200 250 255 ], [ 2 3 4 5 6 7 ], 1)               # three channels --> 0.0223034734917733   0.7641681901279727


## 5050 WS2811B LEDs from Ali:
led1n    = 10;
led1v    = [  0   10   50  100  150  200  250  255 ];
led1mRGB = [  58  71  123  188  252  316  381  388 ] ./ led1n;
led1mR   = [  58  62   80  102  124  147  169  171 ] ./ led1n;
led1mG   = [  58  62   80  102  125  147  169  172 ] ./ led1n;
led1mB   = [  58  62   81  103  126  148  170  173 ] ./ led1n;
led1mRG  = [  58  67  102  145  189  233  276  281 ] ./ led1n;
led1mRB  = [  58  67  102  146  190  234  277  283 ] ./ led1n;
led1mGB  = [  58  67  102  146  190  234  278  283 ] ./ led1n;


led1pRGB = polyfit(led1v, led1mRGB, 1)
led1pR   = polyfit(led1v, led1mR,   1)
led1pG   = polyfit(led1v, led1mG,   1)
led1pB   = polyfit(led1v, led1mB,   1)
led1pRG  = polyfit(led1v, led1mRG,  1)
led1pRB  = polyfit(led1v, led1mRB,  1)
led1pGB  = polyfit(led1v, led1mGB,  1)

led1eRGB = polyval(led1pRGB, led1v);
led1eR   = polyval(led1pR,   led1v);
led1eG   = polyval(led1pG,   led1v);
led1eB   = polyval(led1pB,   led1v);
led1eRG  = polyval(led1pRG,  led1v);
led1eRB  = polyval(led1pRB,  led1v);
led1eGB  = polyval(led1pGB,  led1v);

plot(
    led1v, led1eRGB, "-k;;",
    led1v, led1eR,   "-r;;",
    led1v, led1eG,   "-g;;",
    led1v, led1eB,   "-b;;",
    led1v, led1eRG,  "-m;;",
    led1v, led1eRB,  "-c;;",
    led1v, led1eGB,  "-k;;",
    led1v, led1mRGB, "xk;RGB;",
    led1v, led1mR,   "*r;R;",
    led1v, led1mG,   "+g;G;",
    led1v, led1mB,   "xb;B;",
    led1v, led1mRG,  "xm;RG;",
    led1v, led1mRB,  "xc;RB;",
    led1v, led1mGB,  "xk;GB;"
    );
title("5050 WS2811B LEDs");
xlabel("value");
ylabel("mA");
grid on;
