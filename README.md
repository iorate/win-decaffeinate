# win-decaffeinate
A command for Windows that performs the "opposite" of [win-caffeinate](https://github.com/iorate/win-caffeinate). It can put the display and system to sleep.

## Example
```
$ decaffeinate -d
```

`decaffeinate` puts the display to sleep.

```
$ decaffeinate -t 3600
```

`decaffeinate` puts the system to sleep and awakes it in 3600 seconds.

## Requirements
To build it, a compiler that supports C++17 is required.

## Usage
```
$ decaffeinate --help
Usage: decaffeinate [OPTION...]
  put the system to sleep

Options:
  -h         --help            display this help and exit
  -v         --version         display the version info and exit
  -d         --display         put only the display to sleep
  -t PERIOD  --timeout=PERIOD  specify the timeout value in seconds
```

## Author
[iorate](https://github.com/iorate) ([Twitter](https://twitter.com/iorate))

## License
[Boost Software License 1.0](LICENSE_1_0.txt)
