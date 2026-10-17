# http

An HTTP server written from scratch in C. One file, no dependencies beyond libc.

It listens on port 8080, accepts one connection at a time, reads the request, and
answers a `GET` with a hardcoded `200 OK` plain-text response. Anything that isn't
a `GET` is logged to stdout and the connection is closed.

## Build

```
cc -o http src/server.c
```

## Run

```
./http
curl localhost:8080
```

## Current limitations

- Only `GET` is handled; other methods get no response.
- Request headers are read but never parsed, so there is no routing or file serving.
- The whole request is assumed to arrive in a single `recv` of up to 1000 bytes.
- Connections are handled serially — no `fork`, no threads, no `select`.

Built as a socket-programming exercise to see how much of HTTP you actually need
before a client is satisfied.
