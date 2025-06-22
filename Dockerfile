FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    wget ca-certificates libssl3 libstdc++6 && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /server

RUN wget https://github.com/GlobedGD/globed2/releases/download/v1.8.5/globed-central-server-x64 -O central-server && \
    wget https://github.com/GlobedGD/globed2/releases/download/v1.8.5/globed-game-server-x64 -O game-server && \
    chmod +x central-server game-server

COPY server/central-conf.json .

EXPOSE 14242 14243

CMD ["sh", "-c", "./central-server & ./game-server"]
