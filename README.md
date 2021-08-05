# tcplat

measuring latency between client and server

```
./lats <listen-port> <n-dots>

./latc <dest-ip> <dest-port> <nr-connections>
```

server just listens, client connects to server, sends 4 bytes and wait 4 bytes, closing. repeat nr-connections times and measure time.

warning - this is a 10 minute hack without mutch argument checking
