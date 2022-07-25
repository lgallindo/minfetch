# minfetch: system fetch program focused on multi-threading

![screenshot](https://github.com/rarafael/minfetch/blob/60e640aed6f6814a5538523d51019317dfd12328/screenshot.png)

I wanted an excuse to make something and I found neofetch's running times
quite slow, so I tried reinplementing (some) of it under C with some threads 
thrown in for faster performance. `time` tells me it runs in about 1~3ms

**NOTE: this cannot be built outside of linux at the moment**

### building:
```
git clone https://github.com/rarafael/minfetch.git
make
```

### installing:
the default installation directory is under ~/.local/bin
```
git clone https://github.com/rarafael/minfetch.git
make install
```

### uninstalling:
```
make uninstall
```
