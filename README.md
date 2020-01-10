# nostale-packet-sender
A packet sender for the MMORPG NosTale that reads packets from a pipe and sends them to the game server using the game client's send function. You can find a minimal example of a pipe server [here](https://gist.github.com/Gilgames000/dbdacd1f455aff60d819ffdddd3dfd02).

## How to use
1. Build the dll
2. Launch NosTale game client
3. [Inject](https://www.google.com/search?q=how+to+inject+a+dll) into `NostaleClientX.exe`
using your preferred dll injector
4. Open your custom named pipe server and write to the pipe `\\.\pipe\nt_snd_42`

## How to build
1. Make sure you have Visual Studio 2019 or Build Tools installed with the C++ Workload
2. Clone or download the repository
3. Open the Developer Command Prompt for VS 2019 (x86) and navigate to the project's root directory
4. Run `nmake build` to build the project, `nmake build DEBUG=yes` if you want to enable the debug console, or `nmake build DEBUG=yes LOG=yes` if you also want to log the packets 
5. You can find the dll in the `build` folder

## License
This project is licensed under the MIT License - see the [LICENSE](https://github.com/Gilgames000/nostale-packet-publisher/blob/master/LICENSE) file for details

## Acknowledgements 
- [nostale-packet-logger](https://github.com/Gilgames000/nostale-packet-logger)
- [nostale-packet-publisher](https://github.com/Gilgames000/nostale-packet-publisher)
- Shoutouts to [@RagonEbker](https://github.com/RagonEbker) for bringing up the packet sender idea
