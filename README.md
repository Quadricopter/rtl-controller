# rtl-controller
Simple rtl_tcp SDR client

`rtl_ctl` allow user to "set" a `rtl_tcp` server central frequency.  
Typical usage is found when using [rtlmux](https://github.com/slepp/rtlmux) and different bands over the day

Compilation
--
```
git clone https://github.com/Quadricopter/rtl-ctl.git  
cd rtl-ctl  
make
```

Usage by example
--
Set the receiver on weather-balloon bands  
Set **401.5 MHz** central frequency with **2.4 MHz** sampling rate
```
rtl_ctl -a rtl_tcp-host-ip -p 1234 -f 401500000 -g 2400000
```
