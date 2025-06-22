FROM ubuntu:22.04

# Install system dependencies and Node.js
RUN apt-get update && apt-get install -y \
    wget curl ca-certificates libssl3 libstdc++6 unzip gnupg && \
    curl -fsSL https://deb.nodesource.com/setup_18.x | bash - && \
    apt-get install -y nodejs && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /server

# Download central and game servers
RUN wget https://github.com/GlobedGD/globed2/releases/download/v1.8.5/globed-central-server-x64 -O central-server && \
    wget https://github.com/GlobedGD/globed2/releases/download/v1.8.5/globed-game-server-x64 -O game-server && \
    chmod +x central-server game-server

# Copy your config and Node project files
COPY server/central-conf.json .
COPY server/package.json .
COPY server/ngrok.js .

# Install ngrok via npm
RUN npm install

# Set your ngrok auth token
ENV NGROK_AUTHTOKEN=2yroyHpNBkP4xNb1Bh7lx9hS7OW_87PUhwVTFKfUZcbCJRaXd

# Configure ngrok with the auth token
RUN npx ngrok config add-authtoken $NGROK_AUTHTOKEN

EXPOSE 14242 14243

# Start ngrok.js to show URL, then run the game servers
CMD sh -c "node ngrok.js & ./central-server & ./game-server"
