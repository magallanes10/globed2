FROM ubuntu:22.04

# Install required dependencies
RUN apt-get update && apt-get install -y \
    wget curl ca-certificates libssl3 libstdc++6 unzip && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /server

# Download central and game servers
RUN wget https://github.com/GlobedGD/globed2/releases/download/v1.8.5/globed-central-server-x64 -O central-server && \
    wget https://github.com/GlobedGD/globed2/releases/download/v1.8.5/globed-game-server-x64 -O game-server && \
    chmod +x central-server game-server

# Copy your central config
COPY server/central-conf.json .

# Expose ports
EXPOSE 4201 4202

# Start central and game server
CMD sh -c "./central-server & ./game-server"
