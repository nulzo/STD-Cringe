
<p align="center">
  <a href="https://github.com/nulzo/STD-Cringe">
    <img src="https://github.com/nulzo/STD-Cringe/raw/main/docs/std-cringe-logo.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">std::cringe</h3>

  <p align="center">
    A multithreaded and based discord bot, written in pure C++20.
    <br/>
    <br/>
  </p>
</p>

![Contributors](https://img.shields.io/github/contributors/nulzo/STD-Cringe?color=dark-green) ![Issues](https://img.shields.io/github/issues/nulzo/STD-Cringe) ![License](https://img.shields.io/github/license/nulzo/STD-Cringe) 

## Table Of Contents

* [About the Project](#about-the-project)
* [Built With](#built-with)
* [Getting Started](#getting-started)
  * [Packages](#packages)
  * [Installation](#installation)
* [Usage](#usage)
* [License](#license)
* [Authors](#authors)

## About The Project

This bot comes as a result to show up my good friend, @etchris. We have previously worked on Discord bots together, but we have always found one issue with them: *the libs are so slow and heavy*.

We both run our bots of raspberry pis, and we found that the Python library was slow and heavy. Therefore, we challenged ourselves to make a quick and lightweight bots that can be served from even the lowest tier hardware. Thus, out of this challenge, std::cringe was born.

Here's why std::cringe is better than any other bot on the market:

1. It uses *local* LLM's (goodbye, OpenAI)! You can say anything to the bot, and he will respond in an ungoverened way. This data is not cached (locally *or* remotely), so you can ensure your conversations are personal...
2. Not everyone can afford to set up these locally, so I also built in a way to call OpenAI if you so desire. If you decide to take this route, you have the freedom to choose the model and the system role, as well.
3. We call a lot of API's. You can find reddit posts, generate images from prompt, have cringe describe what he sees in an image, etc. These are easily extendable through custom functions I've written.
4. It is **fast** (and I mean fast). The bottleneck will almost always be your internet speed, or the hardware your LLM is running on.
5. It is **lightweight**. When livestreaming audio at 128k opus, chatting with the LLM, and making external API calls, you can expect to see, on average, < 0.5mb of memory utilization. The Python implementation, when doing half the tasks, was nearly 5x this number.
6. It is multithreaded. Yes, it is true!

Of course, this is C++ we are talking about. You can surely expect some undefined behavior every now and then. But, hey, that just makes it all the more fun ;)

## Built With

C++, CMake, and the love of my life: CLion <3

## Getting Started

Ok -- take a deep breath... This part is a bit long... Let's start with the packages.

### Packages

I run Linux (because I'm goated?). Here is what you need, with installation down below.

* ffmpeg
* opus
* avcodec
* fmt
* openssl
* dpp
* curl
* spdlog
* wget
* g++
* gcc
* cmake
* yt-dlp
* ollama


Ok, I know that seems like a lot, but I emplore you to consider the alternatives. Anyway, here is the commands to install these on Ubuntu:

```sh
sudo add-apt-repository universe

sudo apt-get update

sudo apt-get install libssl-dev libfmt-dev libssl-dev g++ cmake ffmpeg libspdlog-dev opus-tools libopus-dev libspdlog-dev libfmt-dev libssl-dev libavformat-dev libavcodec-dev libavutil-dev libavfilter-dev libcurl4-openssl-dev yt-dlp

sudo apt install wget
wget -O dpp.deb https://dl.dpp.dev/
sudo dpkg -i dpp.deb
```

To get ollama, just go to their github and follow the instructions there. It is very simple, and their documentation is fantastic.

### Installation

1. Clone this repo, naviagte to the directory, and find the build directory. There you will find a file called `env.dist`. Go ahead and copy that to a file in the same directory and call the file `.env`. Nano into the file and add the required fields.
2. In this build dir, run `cmake .. && make` and pray that it builds! You will probably need to troubleshoot based on your specific OS. Download any packages it says you may be missing. I found that UNIX like systems are the easiest, and I have the most trouble with windows. They insist you use visual studio, which I will not do.
3. Once you build, just run `./cringe` and watch the bot come to life!

## Usage
There are so many slash commands, I can't name them all here. Maybe I will some other time, but I'm lazy right now...

## License

Distributed under the MIT License. See [LICENSE](https://github.com/nulzo/STD-Cringe/blob/main/LICENSE.md) for more information.

## Authors

* **Nulzo** - *Computer Science Student* - [nulzo](https://github.com/nulzo/) - *Core developer and designer*
