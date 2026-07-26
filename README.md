# Packet Sniffer

A lightweight, dependency-free network packet sniffer written in C for Linux.
It opens a raw socket, captures traffic straight off the wire, and prints a
human-readable, tree-formatted breakdown of each packet as it decodes it —
layer by layer, the same way a tool like `tcpdump` works under the hood.

No `libpcap`, no external libraries — just raw sockets and hand-written
struct parsing.

```
Packet Size : 74 bytes

Ethernet
 ├─ Destination :  aa:bb:cc:dd:ee:ff
 ├─ Source      :  11:22:33:44:55:66
 └─ Protocol    :  IPv4 (0x0800)

IPv4 Header
 ├─ Version      : 4
 ├─ Header Length: 20 Bytes
 ├─ Protocol     : Payload: TCP
 ├─ Total Length : 60 Bytes
 ├─ TTL          : 64
 ├─ Source IP    : 192.168.1.10
 └─  Destination  : 93.184.216.34

TCP
 ├─ Header Length                : 40 Bytes
 ├─ Source Port                  : 51342
 ├─ Destination Port             : HTTPS
 ├─ Sequence Number              : 123456789
 ├─ Acknowledgement Number       : 0
 ├─ Window Size                  : 64240
 ├─ CheckSum                     : 0x1A2B
 ├─ Point Urgent                 : 0
 └─ Flags                        : SYN
------------------------------------------------------------
```

## What it supports

| Layer | Protocol | Status |
|---|---|---|
| Link | Ethernet II | ✅ src/dst MAC, EtherType |
| Network | IPv4 | ✅ full header, options-aware length check |
| Network | IPv6 | ✅ fixed header (no extension-header walking yet) |
| Transport | TCP (v4 & v6) | ✅ ports, seq/ack, window, flags, checksum |
| Transport | UDP (v4 & v6) | ✅ ports, length, checksum |
| Transport | ICMP / ICMPv6 | ✅ type, code, checksum (raw values only) |
| Application | DNS | ✅ header, question, A-record answers |
| Link | ARP | 🔲 planned — empty `ethernet/arp/` folder reserved for this |

## Project layout

The folder structure mirrors the actual packet encapsulation, which makes it
easy to find the parser for any given layer:

```
packet_sniffer/
├── main.c                        # Opens the raw socket, runs the capture loop
├── Makefile                      # Auto-discovers all .c files, builds `sniffer`
└── ethernet/
    ├── ethernet.c / .h           # Ethernet II header parsing
    ├── arp/                      # Reserved for future ARP support (currently empty)
    ├── ipv4/
    │   ├── ipv4.c / .h           # IPv4 header parsing + protocol dispatch
    │   ├── protocol.c / .h       # EtherType / IP-protocol number → name lookups
    │   ├── icmp4/                # ICMPv4 parsing
    │   ├── tcp4/                 # TCP-over-IPv4 parsing
    │   └── udp4/
    │       ├── udp4.c / .h       # UDP-over-IPv4 parsing
    │       └── dns/              # DNS message parsing (triggered on port 53)
    └── ipv6/
        ├── ipv6.c / .h           # IPv6 header parsing + next-header dispatch
        ├── icmp6/                # ICMPv6 parsing
        ├── tcp6/                 # TCP-over-IPv6 parsing
        └── udp6/                 # UDP-over-IPv6 parsing
```

Every source file now has a header comment explaining its purpose, and every
function has a doc comment describing what it does, its parameters, and any
assumptions/bounds-checks it relies on.

## How it works, in short

1. `main()` opens an `AF_PACKET` / `SOCK_RAW` socket with `ETH_P_ALL`, which
   captures every frame on every interface, bypassing the normal socket API.
2. Each captured frame is handed to `parse_ethernet()`, which prints the MACs
   and returns the EtherType.
3. Based on the EtherType, the frame's payload (everything after the 14-byte
   Ethernet header) is handed to `parse_ipv4()` or `parse_ipv6()`.
4. The IP-layer parser prints its header fields and, based on the
   protocol/next-header value, dispatches the remaining bytes to the matching
   transport parser (`parse_tcp`, `parse_udp`, or `parse_icmp`).
5. `parse_udp()` additionally recognizes DNS traffic (port 53) and hands the
   UDP payload to `parse_dns()`.

Throughout, each parser performs a length check before casting raw bytes to a
struct, to avoid reading past the end of the captured frame.

## Building & running

Requires `gcc` and a Linux kernel (this uses Linux-specific raw-socket APIs,
so it will not build on macOS/Windows without changes).

```bash
make          # builds ./sniffer
sudo ./sniffer   # or: sudo make run
```

Raw packet sockets require root privileges or the `CAP_NET_RAW` capability,
which is why `sudo` is needed. The program runs until you stop it with
`Ctrl+C` (there's currently no clean shutdown handler — see Known Issues).

```bash
make clean    # removes all .o/.d files and the sniffer binary
```

## Known issues & limitations

These are honest notes on the current state of the code, worth keeping in
mind before extending it or relying on it for anything beyond learning/local
testing:

- **DNS parsing has no bounds-checking while walking variable-length
  records.** `parse_question`/`parse_answer` advance through the buffer
  based on length bytes read from the packet itself, without re-checking
  against the remaining buffer size. A malformed or malicious DNS packet
  could cause an out-of-bounds read. Don't point this at untrusted/hostile
  traffic without hardening this first.
- **DNS name compression is not handled.** Compressed domain names (a
  pointer back into the message instead of a literal label) will not decode
  correctly.
- **IPv6 extension headers aren't walked.** If the Next Header field names
  an extension header (Hop-by-Hop Options, Routing, Fragment, etc.) instead
  of a transport protocol, the parser reports "Unknown next header" instead
  of skipping past it to the real payload.
- **No checksum verification** — checksums are printed but not validated.
- **tcp4.c and tcp6.c are almost entirely duplicated code.** A future
  refactor could share one TCP-printing function between both.
- **No CLI arguments** — no interface selection, no BPF-style filter, no
  file output (e.g. pcap format). Currently captures everything, on every
  interface, and only prints to stdout.
- **No signal handling** for a graceful shutdown (Ctrl+C just kills the
  process; the OS reclaims the socket on exit, so this isn't a resource
  leak in practice, just not a clean exit path).
- Minor typos in some printed strings ("Incomplet", "pylod") — cosmetic
  only, doesn't affect parsing.

### Fixed while documenting this codebase
- `ethernet/ipv6/udp6/udp6.h` used the include guard `UDP4_H`, colliding
  with `ethernet/ipv4/udp4/udp4.h`. Harmless today since nothing includes
  both headers together, but a real latent bug — fixed to `UDP6_H`.

## Roadmap ideas

- ARP parsing (folder already reserved at `ethernet/arp/`)
- IPv6 extension header traversal
- Bounds-safe DNS parsing + name-compression support
- Command-line flags: interface selection (`-i eth0`), simple filters
  (`-p tcp`, `--port 443`), and writing captures to a `.pcap` file for
  opening in Wireshark
- Checksum verification
- Shared TCP header printer for IPv4/IPv6 to remove duplication

## License

MIT — see [`LICENSE`](./LICENSE).
