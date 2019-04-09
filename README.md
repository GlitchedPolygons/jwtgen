# jwtgen

## What is it?

Jwtgen is a quick and lightweight jwt generator (console-only). You give it your desired token parameters like issuer, various claims, etc... and it spits out an encoded jwt.

## How to use it

### Windows

* Open `explorer.exe` and navigate to the folder where `jwtgen.exe` resides.
* Click on the navigation bar and replace the path with `cmd` and hit enter. 
* In the command line interface, you can now start jwtgen.exe with your desired arguments
* * `jwtgen.exe -iIssuerName -sJohnDoe --exp=1587399600 --claim=role:admin --claim=age:23 -kSuperSafeHmacSecret123 --alg=HS256`

### Linux/macOS

* Open your terminal and cd into the directory where the jwtgen executable resides.
* Execute jwtgen with your desired token claims and arguments:
* `./jwtgen --iss=IssuerName --sub=JohnDoe --exp=1587399600 --alg=RS512 --copy --key=/home/username/path/to/public-rsa-key.pem`

## What are the parameters

* `-h, --help`
* * Print usage and exit.
* `-c, --copy`
* * Copy generated token to clipboard automatically.
* `-i, --iss`
* * The jwt's issuer (name of who created/signed this token).
* `-s, --sub`
* * The jwt's sub claim (usually whom this token refers to).
* `-a, --aud`
* * The jwt's intended audience (recipients). Should represent who or what this token is intended for. Optional, according to RFC7519.
* `--exp`
* * The jwt's expiration date in numeric date format, meaning the amount of SECONDS SINCE 1970-01-01T00:00:00Z UTC according to the [RFC7519 standard](https://tools.ietf.org/html/rfc7519#section-4.1.4). You can use [unixtimestamp.com](https://unixtimestamp.com) to your advantage.
* `--iat`
* * The numeric date format of when this token was issued. If you don't pass this argument, it defaults to the current time in UTC.
* `--nbf`
* * Datetime of when the jwt starts being valid (in numeric date format, just as in the --exp argument).
* `--claim`
* * Put as many claims in as you need. Specify them with the syntax \"--claim=CLAIM_NAME:CLAIM_VALUE\" (without quotation marks).
* `--alg`
* * The algorithm to use for signing the token. Can be HS256, HS384, HS512, RS256, RS384 or RS512.
* `-k, --key`
* * The secret string to use for signing the token (when selected an HMACSHA algo) __OR__ the file path to the private RSA key used for signing the token (for RSASHA algorithms) - the file must be a text file containing the private key in PEM format. If omitted, the token won't be signed at all (the --alg argument is ignored in that case).
* `-p, --pw`
* * Password for decrypting the RSA key (if the key requires one).

## How to build from source

### Dependencies

* [CMake](https://cmake.org/) >3.10
* [OpenSSL](https://github.com/openssl/openssl/) [>1.1.1c](https://github.com/openssl/openssl/tree/OpenSSL_1_1_1-stable)

Note: I included a .zip with pre-built OpenSSL binaries inside `dependencies/openssl` if you don't wanna build it on your own. Check out the [README.md](https://github.com/GlitchedPolygons/jwtgen/tree/master/dependencies/openssl) inside that folder for more information.

#### Building OpenSSL from source

This is only the default way to approach this, [the full, official guide](https://github.com/openssl/openssl/blob/OpenSSL_1_1_1-stable/INSTALL) on how to build OpenSSL from source obviously contains way more customization options. But if you just want to build jwtgen and carry on with your life, here's a bunch of quick instructions on how to build it on the various platforms:

##### Windows (64-bit)

To build OpenSSL on Windows, you need to have [ActiveState Perl](https://www.activestate.com/products/activeperl/) installed, [NASM](https://www.nasm.us) and MSVC on Windows or GCC/G++ on Unix respectively. 

Open the x64 developer's command prompt: open start menu, search for "x64" and then run the native dev command prompt as an administrator. **Make absolutely sure that you are on x64!**

```
cd C:\path\to\where\your\third-party\git\repos\are
git clone https://github.com/openssl/openssl
cd openssl
git checkout OpenSSL_1_1_1-stable
perl Configure VC-WIN64A
nmake
nmake test
nmake install
```

##### Linux and macOS (64-bit)

To build OpenSSL on Unix, make sure that you have your compilers set up (GCC/G++). 
E.g. `sudo apt-get install build-essential` or the XCode/command-line tools on macOS
```
cd ~/path/to/where/you/store/your/third-party/git-repos
git clone https://github.com/openssl/openssl
cd openssl
git checkout OpenSSL_1_1_1-stable
./config
make
make test
sudo make install
```

### Build jwtgen

* Make sure you have CMake installed on your system (either build it from source and add it to your environment's path or use pre-built binaries).
* Open `cmd.exe` and navigate to a folder where you want jwtgen to clone into.
* `git clone https://github.com/GlitchedPolygons/jwtgen`
* `cd jwtgen`
* `mkdir build && cd build`
* `cmake ..`
* * `make` for Linux or macOS 
* * For Windows, use msbuild or just open `jwtgen.sln` in Visual Studio, select `Release mode (x64)` and build the solution from there.
* * * On Windows, you need to **copy** OpenSSL's `libcrypto-1_1-x64.dll` from the OpenSSL installation path's `bin/` folder into `jwtgen/build/Release`
* * * Usually, the OpenSSL installation path on Windows is `C:\Program Files\OpenSSL`
