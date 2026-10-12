# QuickSock Chat

QuickSock is a small Windows chat application built with the Winsock 2 API. It includes a server that accepts multiple client connections and broadcasts messages to all connected users, along with a client that connects to the server and sends messages.

## Project structure

- `src/server.cpp` - chat server implementation
- `src/client.cpp` - chat client implementation
- `bin/` - generated build output
- `Makefile` - build targets for the project

## Requirements

- Windows 10 or later
- MinGW-w64 or another C++ compiler with WinSock support
- `make` (or `mingw32-make` on MinGW)

## Build

From the project root, run:

```bash
mingw32-make
```

If your MinGW installation provides GNU Make as `make`, use `make` instead.

This creates the executables in the `bin/` directory:

- `bin/server.exe`
- `bin/client.exe`

## Run the application

1. Start the server:

```bash
bin\server.exe
```

2. In a second terminal, start the client:

```bash
bin\client.exe
```

3. Enter a chat name when prompted, then send messages from the client.

## Notes

- The server listens on port `54000`.
- The client connects to `127.0.0.1` by default.
- The chat supports a `/quit` command to leave the conversation.
