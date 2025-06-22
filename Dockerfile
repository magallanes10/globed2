FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    wget curl ca-certificates libssl3 libstdc++6 unzip gnupg && \
    curl -fsSL https://deb.nodesource.com/setup_18.x | bash - && \
    apt-get install -y nodejs && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /server

RUN wget https://github.com/GlobedGD/globed2/releases/download/v1.8.5/globed-central-server-x64 -O central-server && \
    wget https://github.com/GlobedGD/globed2/releases/download/v1.8.5/globed-game-server-x64 -O game-server && \
    chmod +x central-server game-server

COPY server/central-conf.json .

# Install ngrok
RUN curl -s https://ngrok-agent.s3.amazonaws.com/ngrok.asc | tee /etc/apt/trusted.gpg.d/ngrok.asc >/dev/null && \
    echo "deb https://ngrok-agent.s3.amazonaws.com buster main" | tee /etc/apt/sources.list.d/ngrok.list && \
    apt-get update && apt-get install -y ngrok

# Set your ngrok auth token (pass this securely or replace here)
ENV NGROK_AUTHTOKEN=2yroyHpNBkP4xNb1Bh7lx9hS7OW_87PUhwVTFKfUZcbCJRaXd

# Configure ngrok
RUN ngrok config add-authtoken $NGROK_AUTHTOKEN

EXPOSE 14242 14243

# Run ngrok silently and start both game servers
CMD sh -c "node ngrok.js & ./central-server & ./game-server"
