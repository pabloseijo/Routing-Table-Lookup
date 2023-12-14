# 🌐 Routing Table Lookup Project

## 📖 Description

This project implements a C program to read and analyze a routing table (also known as a forwarding table) of a router. The program determines the appropriate output interface for a given IP address, using the Longest Prefix Match rule.

### 🔍 Longest Prefix Rule

- **IP Prefix:** In an IP address, the prefix refers to the initial part of the address that identifies the network to which the host belongs. For example, in 192.168.1.0/24, the prefix would be 192.168.1, and /24 indicates that the first 24 bits of the address are the prefix.
- **📏 Longest Match:** This means that, of all possible matches, the one with the longest prefix is chosen.

### 🌉 Output Interface

- In networking, an interface is a connection point between two systems, which can be physical (e.g., Ethernet) or virtual. It's where data must be sent to reach its destination.

### 📚 Reading the Routing Table

- A routing table is a list used by a router or software to determine where to send data.
- It contains information about networks and which output interface corresponds to each one. For example, 193.144.0.0/16,3 means that for the network 193.144.0.0 with a 16-bit prefix, the output interface is 3.

### 🎭 Network Mask

- A network mask is a bit combination that serves to delimit the prefix of an address.

## 🛠️ Functions

- `inet_net_pton` converts a text-based IP address with network prefix notation (like "192.168.1.0/24") to a binary form.
- `inet_net_ntop` performs the reverse operation, converting the binary form to text.

## 🚀 Usage

### Compilation

To compile the `router.c` program, use the following command in your terminal:

```
gcc -o router router.c
```

This command will compile the router.c file using the GCC compiler and produce an executable named router.

### Execution

To run the program, use the following command, where archivo.txt is the name of your routing table file:

```
./router archivo.txt [IP Address]
```
Replace [IP Address] with the IP address for which you want to find the output interface. For example:

```
./router archivo.txt 192.168.1.100
```

## 📌 Prerequisites

- A C compiler (e.g., GCC)
- Standard C libraries

## 📜 License

- Distributed under the MIT License. See `LICENSE` for more information.
