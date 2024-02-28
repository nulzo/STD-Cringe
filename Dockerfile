# Use an official base image with a C++ environment
FROM gcc:13-bookworm

# Set the working directory inside the container
WORKDIR /usr/src/app

# Copy the necessary files into the container
COPY . .

WORKDIR /usr/src/app/cmake-build

#RUN apt-get update && apt-get install -y cmake libssl-dev libfmt-dev libssl-dev g++ cmake ffmpeg libspdlog-dev opus-tools libopus-dev libspdlog-dev libfmt-dev libssl-dev libavformat-dev libavcodec-dev libavutil-dev libavfilter-dev libcurl4-openssl-dev yt-dlp
RUN apt-get update && apt-get install -y wget libssl-dev cmake zlib1g-dev libsodium-dev libopus-dev ffmpeg libspdlog-dev opus-tools libopus-dev libspdlog-dev libfmt-dev libssl-dev libavformat-dev libavcodec-dev libavutil-dev libavfilter-dev libcurl4-openssl-dev yt-dlp
RUN wget -O dpp.deb https://dl.dpp.dev/latest/linux-rpi-arm64
RUN dpkg -i dpp.deb
RUN rm dpp.deb

RUN cmake ..
RUN make

# Set the entry point for the container
CMD ["./cringe"]
